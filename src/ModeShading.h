#pragma once
#include "ModeBase.h"


enum ModeShadingNotAllowedReason : uint32_t
{
    ModeShadingNotAllowedReasonNone = 0,
    ModeShadingNotAllowedReasonSwitchedOff = 1,
    ModeShadingNotAllowedReasonChannelLock = 2,
    ModeShadingNotAllowedReasonTimeNotValid = 4,
    ModeShadingNotAllowedReasonLock = 8,
    ModeShadingNotAllowedReasonStartWaitTime = 16,
    ModeShadingNotAllowedReasonSunAzimut = 32,
    ModeShadingNotAllowedReasonSunElevation = 64,
    ModeShadingNotAllowedReasonSunBreak = 128,
    ModeShadingNotAllowedReasonWindowOpen = 256,
    ModeShadingNotAllowedReasonManualUsage = 512,  
    ModeShadingNotAllowedReasonTemperature = 1024,
    ModeShadingNotAllowedReasonTemperatureForecase = 2048,
    ModeShadingNotAllowedReasonBrightness = 4096,
    ModeShadingNotAllowedReasonUVI = 8192,
    ModeShadingNotAllowedReasonRain =  16384,
    ModeShadingNotAllowedReasonClouds = 32768,
    ModeShadingNotAllowedReasonRoomTemperature = 65536,
    ModeShadingNotAllowedReasonHeating = 131072,
    ModeShadingNotAllowedReasonHeatingInThePast = 262144,
    ModeShadingNotAllowedReasonShutterPosition = 524288,
};

class ModeShading : public ModeBase
{
    std::string _name;
    uint8_t _index;
    uint32_t _notAllowedReason = ModeShadingNotAllowedReason::ModeShadingNotAllowedReasonNone;
    uint32_t _lastNotAllowedReason = 1;
    bool _recalcMeasurmentValues = true;
    bool _allowedByMeasurementValues = false;
    bool _lockActive = false;
    bool _active = false;
    unsigned long _waitTimeAfterMeasurmentValueChange = 0;
    bool _lastSunFrameAllowed = false;
    bool _needWaitTime = false;
    unsigned long _lastHeadingTimeStamp = 0;
    bool _heatingOff = true;
    int16_t koChannelOffset();
    GroupObject& getKo(uint8_t ko);
    bool allowedByMeasurmentValues(const CallContext& callContext);
    bool handleMeasurmentValue(bool& allowed, bool enabled, const MeasurementWatchdog *measurementWatchdog, const CallContext &callContext, bool (*predicate)(const MeasurementWatchdog *, uint8_t _channelIndex, uint8_t _index), ModeShadingNotAllowedReason reasonBit);

public:
    ModeShading(uint8_t index);
    bool allowedBySun(const CallContext& callContext);
protected:
    const char *name() const override;
    uint8_t sceneNumber() const override;
    void initGroupObjects() override;
    bool modeWindowOpenAllowed() const override;
    bool allowed(const CallContext& callContext) override; 
    void start(const CallContext& callContext, const ModeBase* previous, PositionController& positionController) override;
    void control(const CallContext& callContext, PositionController& positionController) override;
    void stop(const CallContext& callContext, const ModeBase* next, PositionController& positionController) override;
    void processInputKo(GroupObject &ko, PositionController& positionController) override;
    bool isModeShading() const override;
};