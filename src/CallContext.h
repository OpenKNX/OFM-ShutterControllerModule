#pragma once

#include "Arduino.h"

class ModeIdle;
class ModeManual;

class CallContext
{
    public:
        bool diagnosticLog;
        unsigned long currentMillis;
        bool timeAndSunValid;
        bool minuteChanged;
        bool summerTime;
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint16_t minuteOfDay;
        uint16_t UtcYear;
        uint8_t UtcMonth;
        uint8_t UtcDay;
        uint8_t UtcHour;
        uint8_t UtcMinute;
        uint16_t UtcMinuteOfDay;
     
        double azimuth;
        double elevation;
        const ModeIdle* modeIdle;
        const ModeManual* modeManual;
};