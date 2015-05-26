#ifndef PidControl_h
#define PidControl_h

#include "Arduino.h"

class PidControl
{
public:
    PidControl(float *kp, float *ki, float *kd);
    float process(float error);

    void reset();
private:
    float *kp;
    float *ki;
    float *kd;
    float integral;
    float lastError;
    uint64_t lastTime;
};

#endif