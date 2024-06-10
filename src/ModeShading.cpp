#include "ModeShading.h"
#include "Timer.h"
#include "Helios.h"

#ifdef SHC_KoCHModeShading2Active
// redefine SHC_ParamCalcIndex to add offset for Shading Mode 2
#undef SHC_ParamCalcIndex
#define SHC_ParamCalcIndex(index) (index + SHC_ParamBlockOffset + _channelIndex * SHC_ParamBlockSize + (SHC_ChannelModeShading2 - SHC_ChannelModeShading1) * (_index - 1))
#endif

ModeShading::ModeShading(uint8_t index)
    : _index(index)
{
    _name = "Shading";
    _name += std::to_string(index);
}

const char *ModeShading::name()
{
    return _name.c_str();
}
void ModeShading::initGroupObjects()
{
    getKo(SHC_KoCHModeShading1LockActive).value(false, DPT_Switch);
    getKo(SHC_KoCHModeShading1Active).value(false, DPT_Switch);
    _recalcMeasurmentValues = true;
}
bool ModeShading::allowed(const CallContext& callContext)
{
    bool noWaitTimeCheck = false;
    if (allowedByTimeAndSun(callContext) != _lastTimeAndSunFrameAllowed)
    {
        _lastTimeAndSunFrameAllowed = !_lastTimeAndSunFrameAllowed; 
        noWaitTimeCheck = true;
    }
    if (!_lastTimeAndSunFrameAllowed)
        return false;

    if (_recalcMeasurmentValues)
    {
        _recalcMeasurmentValues = false;
        auto allowedByMeasurementValues = allowedByMeasurmentValues();
        if (_allowedByMeasurementValues != allowedByMeasurementValues)
        {
            if (noWaitTimeCheck)
                _waitTimeAfterMeasurmentValueChange = 0;
            else
                _waitTimeAfterMeasurmentValueChange = callContext.currentMillis;
            _allowedByMeasurementValues = allowedByMeasurementValues;
        }
    }
    if (_waitTimeAfterMeasurmentValueChange != 0)
    {
        bool lastState = !_allowedByMeasurementValues;
        if (lastState)
        {
            // Check end wait time
            auto waitTimeInMinutes = (unsigned long) ParamSHC_ChannelModeShading1WaitTimeStart;
            if (callContext.currentMillis - _waitTimeAfterMeasurmentValueChange < waitTimeInMinutes * 60000)
                return lastState; 
        }
        else
        {
            // Check start wait time
            auto waitTimeInMinutes = (unsigned long) ParamSHC_ChannelModeShading1WaitTimeEnd;
            if (callContext.currentMillis - _waitTimeAfterMeasurmentValueChange < waitTimeInMinutes * 60000)
                return lastState; 
        }
    }
    return _allowedByMeasurementValues;
}

bool ModeShading::allowedByTimeAndSun(const CallContext& callContext)
{
    if (!getKo(SHC_KoCHModeShading1LockActive).value(DPT_Switch))
        return false;

    if (callContext.azimuth < ParamSHC_ChannelModeShading1AzimutMin || callContext.azimuth > ParamSHC_ChannelModeShading1AzimutMax)
        return false;
    if (callContext.elevation < ParamSHC_ChannelModeShading1ElevationMin || callContext.elevation > ParamSHC_ChannelModeShading1ElevationMax)
        return false;

    auto shadingBreak = ParamSHC_ChannelModeShading1ShadingBreak;

    // <Enumeration Text="Deaktiviert" Value="0" Id="%ENID%" />
    // <Enumeration Text="Azimut" Value="1" Id="%ENID%" />
    // <Enumeration Text="Höhenwinkel" Value="2" Id="%ENID%" />
    // <Enumeration Text="Azimut UND Höhenwinkel" Value="3" Id="%ENID%" />
    // <Enumeration Text="Azimut ODER Höhenwinkel" Value="4" Id="%ENID%" />

    switch (shadingBreak)
    {
    case 1:
        if (callContext.azimuth >= ParamSHC_ChannelModeShading1ShadingBreakAzimutMin && callContext.azimuth <= ParamSHC_ChannelModeShading1ShadingBreakAzimutMax)
            return false;
    case 2:
        if (callContext.elevation >= ParamSHC_ChannelModeShading1ShadingBreakElevationMin && callContext.elevation <= ParamSHC_ChannelModeShading1ShadingBreakElevationMax)
            return false;
    case 3:
        if ((callContext.azimuth >= ParamSHC_ChannelModeShading1ShadingBreakAzimutMin && callContext.azimuth <= ParamSHC_ChannelModeShading1ShadingBreakAzimutMax) &&
            (callContext.elevation >= ParamSHC_ChannelModeShading1ShadingBreakElevationMin && callContext.elevation <= ParamSHC_ChannelModeShading1ShadingBreakElevationMax))
            return false;
        break;
    case 4:
        if ((callContext.azimuth >= ParamSHC_ChannelModeShading1ShadingBreakAzimutMin && callContext.azimuth <= ParamSHC_ChannelModeShading1ShadingBreakAzimutMax) ||
            (callContext.elevation >= ParamSHC_ChannelModeShading1ShadingBreakElevationMin && callContext.elevation <= ParamSHC_ChannelModeShading1ShadingBreakElevationMax))
            return false;
        break;
    }
    return true;
}
bool ModeShading::allowedByMeasurmentValues()
{
    if (ParamSHC_HasTemperaturInput && ParamSHC_ChannelModeShading1TemperatureActive && (float)KoSHC_TemperatureInput.value(DPT_Value_Temp) < ParamSHC_ChannelModeShading1TemperatureMin)
        return false;
    if (ParamSHC_HasTemperaturForecastInput && ParamSHC_ChannelModeShading1TemperatureForecast && (float)KoSHC_TemperatureForecastInput.value(DPT_Value_Temp) < ParamSHC_ChannelModeShading1TemperatureForecastMin)
        return false;
    if (ParamSHC_HasBrightnessInput && ParamSHC_ChannelModeShading1BrightnessActiv && (float)KoSHC_BrightnessInput.value(DPT_Value_Lux) < 1000 * ParamSHC_ChannelModeShading1BrightnessMin)
        return false;
    if (ParamSHC_HasUVIInput && ParamSHC_ChannelModeShading1UVIActiv && (float)KoSHC_UVIInput.value(DPT_DecimalFactor) < ParamSHC_ChannelModeShading1UVIMin)
        return false;
    if (ParamSHC_HasRainInput && ParamSHC_ChannelModeShading1RainActiv && KoSHC_RainInput.value(DPT_Switch))
        return false;
    if (ParamSHC_HasCloudsInput && (uint8_t)KoSHC_CloudsInput.value(DPT_Percent_U8) > ParamSHC_ChannelModeShading1Clouds)
        return false;

    return false;
}

#ifndef SHC_KoCHModeShading2Active
#define SHC_KoCHModeShading2Active SHC_KoCHModeShading1Active
#endif

int16_t ModeShading::koChannelOffset()
{
    return (_index - 1) * (SHC_KoCHModeShading2Active - SHC_KoCHModeShading1Active);
}

GroupObject &ModeShading::getKo(uint8_t ko)
{
    return knx.getGroupObject(ko + koChannelOffset());
}

void ModeShading::start()
{
    _active = true;
    getKo(SHC_KoCHModeShading1Active).value(true, DPT_Switch);
}
void ModeShading::stop()
{
    _active = false;
    _waitTimeAfterMeasurmentValueChange = 0;
    getKo(SHC_KoCHModeShading1Active).value(false, DPT_Switch);
}
void ModeShading::processInputKo(GroupObject &ko)
{
    // channel ko
    switch (ko.asap() - koChannelOffset())
    {
    case SHC_KoCHModeShading1Lock:
        getKo(SHC_KoCHModeShading1LockActive).value(ko.value(DPT_Switch), DPT_Switch);
        _recalcMeasurmentValues = true;
        break;
    }
   _recalcMeasurmentValues = true;
}