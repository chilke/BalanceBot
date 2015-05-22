#include "PidControl.h"

PidControl::PidControl(float *kp, float *ki, float *kd) : kp(kp), ki(ki), kd(kd)
{
    lastTime = millis();
}

float PidControl::process(float error)
{

}