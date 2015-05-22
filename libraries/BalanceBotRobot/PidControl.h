#ifndef PidControl_h
#define PidControl_h

#include "Arduino.h"

class PidControl
{
public:
    PidControl(float *kp, float *ki, float *kd);
    float process(float error);
private:
    float *kp;
    float *ki;
    float *kd;
    float lastError;
    uint64_t lastTime;
};

#endif