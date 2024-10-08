#pragma once

#include "Arduino.h"

class ModeIdle;
class ModeManual;
class ModeBase;
class MeasurementWatchdog;
class PositionController;

class CallContext
{
    public:
        bool modeNewStarted = false;
        bool fastSimulationActive = false;
        bool isWindowOpenActive = false;
        bool hasSlat = false;
        bool diagnosticLog = false;
        bool shadingControlActive = false;
        bool shadingPeriodActive = false;
        bool channelLockActive = false;
        unsigned long currentMillis = 0;
        bool timeAndSunValid = false;
        bool minuteChanged = false;
        bool summerTime = false;
        bool reactivateShadingWaitTimeRunning = false;
        bool reactivateShadingAfterPeriod = false;
        bool shadingDailyActivation = false;
        uint16_t year = 0;
        uint8_t month = 0;
        uint8_t day = 0;
        uint8_t hour = 0;
        uint8_t minute = 0;
        uint16_t minuteOfDay = 0;
        uint16_t utcYear = 0;
        uint8_t utcMonth = 0;
        uint8_t utcDay = 0;
        uint8_t utcHour = 0;
        uint8_t utcMinute = 0;
        uint16_t utcMinuteOfDay = 0;
     
        double azimuth = 0;
        double elevation = 0;
        const ModeIdle* modeIdle = nullptr;
        const ModeManual* modeManual = nullptr;
        const ModeBase* modeCurrentActive = nullptr;
        MeasurementWatchdog* measurementTemperature = nullptr;
        MeasurementWatchdog* measurementTemperatureForecast = nullptr;
        MeasurementWatchdog* measurementBrightness = nullptr;
        MeasurementWatchdog* measurementUVIndex = nullptr;
        MeasurementWatchdog* measurementRain = nullptr;
        MeasurementWatchdog* measurementClouds = nullptr;
        MeasurementWatchdog* measurementRoomTemperature = nullptr;
        MeasurementWatchdog* measurementHeading = nullptr;
        const PositionController* positionController = nullptr;
};