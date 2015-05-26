#include "PidControl.h"
#include "BotDefines.h"

PidControl::PidControl(float *kp, float *ki, float *kd) : kp(kp), ki(ki), kd(kd)
{
    integral = 0;
    lastError = 0;
    lastTime = millis();
}

float PidControl::process(float error)
{
    float errorSum;

    //Repurpose lastTime for deltaTime
    lastTime = millis() - lastTime;

    errorSum = (error - lastError) / (float)lastTime * *kd;;

    integral += error * (float)lastTime;

    lastTime = millis();

    //TODO - update to be configurable per object
    if (integral > MAX_INTEGRAL)
        {
            integral = MAX_INTEGRAL;
        }
        else if (integral < -MAX_INTEGRAL)
        {
            integral = -MAX_INTEGRAL;
        }

    errorSum += error * *kp;

    errorSum += integral * *ki;

    lastError = error;

    return errorSum;
}

void PidControl::reset()
{
    integral = 0;
    lastError = 0;
    lastTime = millis();
}