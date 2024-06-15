#include "ShutterControllerChannel.h"
#include "ModeManual.h"
#include "ModeWindowOpen.h"
#include "ModeNight.h"
#include "ModeShading.h"

ShutterControllerChannel::ShutterControllerChannel(uint8_t channelIndex)
    : _modes()
{
    _channelIndex = channelIndex;
    char buffer[10] = {0};
    _name = itoa(channelIndex, buffer, 10);
}

const std::string ShutterControllerChannel::name()
{
    return _name;
}

void ShutterControllerChannel::setup()
{
    // add modes, highest priority first
    _manualMode = new ModeManual();
    _modes.push_back(_manualMode);
    if (ParamSHC_ChannelModeWindowOpen)
        _modes.push_back(new ModeWindowOpen());
    if (ParamSHC_ChannelModeNight)
        _modes.push_back(new ModeNight());

// up to 4 instances can be defined of ModeShading in ShutterControllerModule.templ.xml
#ifdef ParamSHC_ChannelModeShading5
#error Not more the 4 instances allowed for ModeShading in ShutterControllerModule.templ.xml
#endif
#ifdef ParamSHC_ChannelModeShading4
    if (ParamSHC_ChannelModeShading4)
        _modes.push_back(new ModeShading(4));
#endif
#ifdef ParamSHC_ChannelModeShading3
    if (ParamSHC_ChannelModeShading3)
        _modes.push_back(new ModeShading(3));
#endif
#ifdef ParamSHC_ChannelModeShading2
    if (ParamSHC_ChannelModeShading2)
        _modes.push_back(new ModeShading(2));
#endif
    if (ParamSHC_ChannelModeShading1)
        _modes.push_back(new ModeShading(1));
}

void ShutterControllerChannel::processInputKo(GroupObject &ko)
{
    // channel ko
    auto index = SHC_KoCalcIndex(ko.asap());
    switch (index)
    {
    }
    for (auto mode : _modes)
    {
        mode->processInputKo(ko);
    }
}

bool ShutterControllerChannel::processCommand(const std::string cmd, bool diagnoseKo, bool &diagnosticLogLoopRequest)
{
    if (cmd == "")
    {
        if (_currentMode == nullptr)
            logInfoP("Not started");
        else
        {
            logInfoP("Current active mode: %s", _currentMode->name());
            diagnosticLogLoopRequest = true;
        }
        return true;
    }
    return false;
}

void ShutterControllerChannel::execute(const CallContext &callContext)
{
    ModeBase *nextMode = nullptr;
    for (auto mode : _modes)
    {
        if (callContext.diagnosticLog)
            logInfoP("Mode %s", mode->name());
        logIndentUp();
        if (mode->allowed(callContext))
        {
            if (callContext.diagnosticLog)
                logInfoP("-> allowed");
            if (nextMode == nullptr)
            {
                nextMode = mode;
                // Do not break because allowed should be called for all modes
                // because it is a replacment for the loop function
            }
        }
        else
        {
            if (callContext.diagnosticLog)
                logInfoP("-> not allowed");
        }
        logIndentDown();
    }
    if (nextMode == nullptr)
        nextMode = _manualMode; // Manual mode can be activated event it's allowed function returns false.
    if (_currentMode != nextMode)
    {
        if (_currentMode != nullptr)
        {
            logDebugP("Changing mode from %s to %s", _currentMode->name(), nextMode->name());
            _currentMode->stop();
        }
        else
        {
            logDebugP("Start mode %s", nextMode->name());
        }
        _currentMode = nextMode;
        _currentMode->start();
    }
}