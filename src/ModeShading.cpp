#include "ModeShading.h"
#include "Timer.h"

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
bool ModeShading::modeWindowOpenAllowed() const
{
    return ParamSHC_ChannelModeShading1WindowOpenAllowed;
}
bool ModeShading::allowed(const CallContext &callContext)
{
    bool allowedTimeAndSun = allowedByTimeAndSun(callContext);
    if (allowedTimeAndSun != _lastTimeAndSunFrameAllowed)
    {
        logDebugP("Allowed by time and sun: %d", (int) allowedTimeAndSun);
        _lastTimeAndSunFrameAllowed = allowedTimeAndSun;
        _needWaitTime = false;
    }
    if (!callContext.diagnosticLog && !_lastTimeAndSunFrameAllowed)
        return false;

    if (_recalcMeasurmentValues || callContext.diagnosticLog)
    {
        _recalcMeasurmentValues = false;
        auto allowedByMeasurementValues = allowedByMeasurmentValues(callContext);
        if (_allowedByMeasurementValues != allowedByMeasurementValues)
        {
            logDebugP("Allowed by measurement values: %d", (int) allowedByMeasurementValues);
            if (_needWaitTime)
                _waitTimeAfterMeasurmentValueChange = callContext.currentMillis;
            else
                _waitTimeAfterMeasurmentValueChange = 0;
            _allowedByMeasurementValues = allowedByMeasurementValues;
            if (allowedByMeasurementValues)
                _needWaitTime = true;
        }
    }
    if (_waitTimeAfterMeasurmentValueChange != 0)
    {
        bool lastState = !_allowedByMeasurementValues;
        if (lastState)
        {
            // Check end wait time
            auto waitTimeInMinutes = (unsigned long)ParamSHC_ChannelModeShading1WaitTimeStart;
            if (callContext.currentMillis - _waitTimeAfterMeasurmentValueChange < waitTimeInMinutes * 60000)
            {
                if (callContext.diagnosticLog)
                    logInfoP("Stop wait time %dms not reached: %dms", waitTimeInMinutes * 60000, (callContext.currentMillis - _waitTimeAfterMeasurmentValueChange));
                return lastState;
            }
        }
        else
        {
            // Check start wait time
            auto waitTimeInMinutes = (unsigned long)ParamSHC_ChannelModeShading1WaitTimeEnd;
            if (callContext.currentMillis - _waitTimeAfterMeasurmentValueChange < waitTimeInMinutes * 60000)
            {
                if (callContext.diagnosticLog)
                    logInfoP("Start wait time %dms not reached: %dms", waitTimeInMinutes * 60000, (callContext.currentMillis - _waitTimeAfterMeasurmentValueChange));
                return lastState;
            }
        }
        _waitTimeAfterMeasurmentValueChange = 0;
    }
    return _allowedByMeasurementValues && _lastTimeAndSunFrameAllowed;
}

bool ModeShading::allowedByTimeAndSun(const CallContext &callContext)
{
    bool diagnosticLog = callContext.diagnosticLog;
    bool allowed = true;
    if (getKo(SHC_KoCHModeShading1LockActive).value(DPT_Switch))
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Lock KO activ");
        else
            return false;
    }
    if (callContext.azimuth < ParamSHC_ChannelModeShading1AzimutMin || callContext.azimuth > ParamSHC_ChannelModeShading1AzimutMax)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Azimut %.2f not between %d and %d", callContext.azimuth, (int) ParamSHC_ChannelModeShading1AzimutMin, (int) ParamSHC_ChannelModeShading1AzimutMax);
        else
            return false;
    }
    if (callContext.elevation < ParamSHC_ChannelModeShading1ElevationMin || callContext.elevation > ParamSHC_ChannelModeShading1ElevationMax)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Elevantion %.2f not between %d and %d", callContext.elevation, (int)ParamSHC_ChannelModeShading1ElevationMin, (int) ParamSHC_ChannelModeShading1ElevationMax);
        else
            return false;
    }

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
        {
            allowed = false;
            if (diagnosticLog)
                logInfoP("Shading break azimut in range");
            else
                return false;
        }
        break;
    case 2:
        if (callContext.elevation >= ParamSHC_ChannelModeShading1ShadingBreakElevationMin && callContext.elevation <= ParamSHC_ChannelModeShading1ShadingBreakElevationMax)
        {
            allowed = false;
            if (diagnosticLog)
                logInfoP("Shading break elevation in range");
            else
                return false;
        }
        break;
    case 3:
        if ((callContext.azimuth >= ParamSHC_ChannelModeShading1ShadingBreakAzimutMin && callContext.azimuth <= ParamSHC_ChannelModeShading1ShadingBreakAzimutMax) &&
            (callContext.elevation >= ParamSHC_ChannelModeShading1ShadingBreakElevationMin && callContext.elevation <= ParamSHC_ChannelModeShading1ShadingBreakElevationMax))
        {
            allowed = false;
            if (diagnosticLog)
                logInfoP("Shading break azimut and elevation in range");
            else
                return false;
        }
        break;
    case 4:
        if ((callContext.azimuth >= ParamSHC_ChannelModeShading1ShadingBreakAzimutMin && callContext.azimuth <= ParamSHC_ChannelModeShading1ShadingBreakAzimutMax) ||
            (callContext.elevation >= ParamSHC_ChannelModeShading1ShadingBreakElevationMin && callContext.elevation <= ParamSHC_ChannelModeShading1ShadingBreakElevationMax))
            return false;
        {
            allowed = false;
            if (diagnosticLog)
                logInfoP("Shading break azimut or elevation in range");
            else
                return false;
        }
        break;
    }
    return allowed;
}
bool ModeShading::allowedByMeasurmentValues(const CallContext &callContext)
{
    bool diagnosticLog = callContext.diagnosticLog;
    bool allowed = true;
    if (ParamSHC_HasTemperaturInput && ParamSHC_ChannelModeShading1TemperatureActive && (float)KoSHC_TemperatureInput.value(DPT_Value_Temp) < ParamSHC_ChannelModeShading1TemperatureMin)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Temperatur %.1f is lower than %d", (double) KoSHC_TemperatureInput.value(DPT_Value_Temp), (int) ParamSHC_ChannelModeShading1TemperatureMin);
        else
            return false;
    }
    if (ParamSHC_HasTemperaturForecastInput && ParamSHC_ChannelModeShading1TemperatureForecast && (float)KoSHC_TemperatureForecastInput.value(DPT_Value_Temp) < ParamSHC_ChannelModeShading1TemperatureForecastMin)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Temperaturforecast %.1f is lower than %d", (double)KoSHC_TemperatureForecastInput.value(DPT_Value_Temp), (int) ParamSHC_ChannelModeShading1TemperatureForecastMin);
        else
            return false;
    }
    if (ParamSHC_HasBrightnessInput && ParamSHC_ChannelModeShading1BrightnessActiv && (float)KoSHC_BrightnessInput.value(DPT_Value_Lux) < 1000 * ParamSHC_ChannelModeShading1BrightnessMin)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Brightness %.1f is lower than %d", (double)KoSHC_BrightnessInput.value(DPT_Value_Lux), (int) 1000 * ParamSHC_ChannelModeShading1BrightnessMin );
        else
            return false;
    }
    if (ParamSHC_HasUVIInput && ParamSHC_ChannelModeShading1UVIActiv && (float)KoSHC_UVIInput.value(DPT_DecimalFactor) < ParamSHC_ChannelModeShading1UVIMin)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("UV index %.1f is lower than %d", (double)KoSHC_UVIInput.value(DPT_DecimalFactor), (int) ParamSHC_ChannelModeShading1UVIMin);
        else
            return false;
    }
    if (ParamSHC_HasRainInput && ParamSHC_ChannelModeShading1RainActiv && KoSHC_RainInput.value(DPT_Switch))
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Rain lock");
        else
            return false;
    }
    if (ParamSHC_HasCloudsInput && (uint8_t)KoSHC_CloudsInput.value(DPT_Scaling) > ParamSHC_ChannelModeShading1Clouds)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Clouds %d more than %d", (int)KoSHC_CloudsInput.value(DPT_Scaling), (int) ParamSHC_ChannelModeShading1Clouds);
        else
            return false;
    }
    if (!callContext.modeCurrentActive->isShading() &&  (uint8_t) KoSHC_CHShutterPercentInput.value(DPT_Scaling) > ParamSHC_ChannelModeShading1ActivationOnlyIfLessThan)
    {
        allowed = false;
        if (diagnosticLog)
            logInfoP("Shutter %d more than %d", (int)KoSHC_CHShutterPercentInput.value(DPT_Scaling), (int) ParamSHC_ChannelModeShading1ActivationOnlyIfLessThan);
        else
            return false;
    }
    return allowed;
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

void ModeShading::start(ModeBase* previous)
{
    _active = true;
    getKo(SHC_KoCHModeShading1Active).value(true, DPT_Switch);
    
    auto shutterPosition = ParamSHC_ChannelModeShading1ShadingPosition;
    KoSHC_CHShutterPercentOutput.value(shutterPosition, DPT_Scaling);
 
    // <Enumeration Text="Kanal deaktiviert" Value="0" Id="%ENID%" />
    // <Enumeration Text="Jalousie" Value="1" Id="%ENID%" />
    // <Enumeration Text="Rollo" Value="2" Id="%ENID%" />
    if (ParamSHC_ChannelType == 1 && !ParamSHC_ChannelModeShading1SlatElevationDepending)
        KoSHC_CHShutterSlatOutput.value(ParamSHC_ChannelModeShading1SlatShadingPosition, DPT_Scaling);

}

void ModeShading::control(const CallContext &callContext)
{
    if (!callContext.newStarted && !callContext.minuteChanged && !callContext.diagnosticLog)
        return;
  
    // <Enumeration Text="Kanal deaktiviert" Value="0" Id="%ENID%" />
    // <Enumeration Text="Jalousie" Value="1" Id="%ENID%" />
    // <Enumeration Text="Rollo" Value="2" Id="%ENID%" />
    if (ParamSHC_ChannelType != 1)
        return;

    // Jalousie    
    if (ParamSHC_ChannelModeShading1SlatElevationDepending)
    {
        auto targetSlatPosition = (90 - callContext.elevation) / 90 * 50 + 50 + (double) ParamSHC_ChannelModeShading1OffsetSlatPosition;
        if (targetSlatPosition < 0)
            targetSlatPosition = 0;
        else if (targetSlatPosition > 100)
            targetSlatPosition = 100;
        auto slatPosition = (uint8_t) targetSlatPosition;
        if (callContext.diagnosticLog)
            logInfoP("Calculated slat position %d", (int) slatPosition);

        if (!callContext.newStarted && abs((int) KoSHC_CHShutterPercentOutput.value(DPT_Scaling) - slatPosition) < ParamSHC_ChannelModeShading2MinChangeForSlatAdaption)
        {
            if (callContext.diagnosticLog)
                logInfoP("Slat position %d difference is less then %d", (int) abs((int) KoSHC_CHShutterPercentOutput.value(DPT_Scaling) - slatPosition), (int) ParamSHC_ChannelModeShading2MinChangeForSlatAdaption);

            return; // Do not change, to less difference
        }   
        
        KoSHC_CHShutterSlatOutput.value(slatPosition, DPT_Scaling);
    }
}

void ModeShading::stop(ModeBase* next)
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

bool ModeShading::isShading() const
{
    return true;
}