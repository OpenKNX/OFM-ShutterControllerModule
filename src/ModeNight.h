#pragma once
#include "ModeBase.h"

class ModeNight : public ModeBase
{
protected:
    const char *name() override;
    void initGroupObjects() override;
    bool allowed() override;
    void start() override;
    void stop() override;
    void processInputKo(GroupObject &ko) override;
};