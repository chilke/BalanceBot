#include "Arduino.h"
#include "Joystick.h"

Joystick::Joystick(uint8_t xPin, uint8_t yPin, uint8_t swPin)
{
    this->xPin = xPin;
    this->yPin = yPin;
    this->swPin = swPin;
    
    calibrationData = NULL;
}

void Joystick::begin()
{
    pinMode(swPin, INPUT_PULLUP);
}

void Joystick::setCalibrationData(JoystickCalibrationData *calData)
{
    calibrationData = calData;
}

void Joystick::read(JoystickData *data)
{
    int32_t value;
    int8_t absX;
    int8_t absY;

    value = analogRead(xPin);
    value -= calibrationData->zeroX;
    if (abs(value) < calibrationData->deadZone)
    {
        value = 0;
    }
    value *= 100;
    if (value < 0)
    {
        value /= (calibrationData->zeroX-calibrationData->minX);
    }
    else
    {
        value /= (calibrationData->maxX-calibrationData->zeroX);
    }

    data->x = value;
    absX = abs(value);
    
    value = analogRead(yPin);
    value -= calibrationData->zeroY;
    if (abs(value) < calibrationData->deadZone)
    {
        value = 0;
    }
    value *= 100;
    if (value < 0)
    {
        value /= (calibrationData->zeroY-calibrationData->minY);
    }
    else
    {
        value /= (calibrationData->maxY-calibrationData->zeroY);
    }

    data->y = value;
    absY = abs(value);

    if (absX < MAX_JOYSTICK_ZEROED && absY < MAX_JOYSTICK_ZEROED)
    {
        data->stickState = SSZeroed;
    }
    else
    {
        //Clear the moved bit in case it was set before
        data->stickState = (StickState)(data->stickState & ~JOYSTICK_MOVED_MASK);

        if (absX > absY + MIN_JOYSTICK_DIFF)
        {
            if (data->x > MIN_JOYSTICK_MOVED && data->stickState != SSRight)
            {
                data->stickState = SSMovedRight;
            }
            else if (data->x < -MIN_JOYSTICK_MOVED && data->stickState != SSLeft)
            {
                data->stickState = SSMovedLeft;
            }
        }
        else if (absY > absX + MIN_JOYSTICK_DIFF)
        {
            if (data->y > MIN_JOYSTICK_MOVED && data->stickState != SSUp)
            {
                data->stickState = SSMovedUp;
            }
            else if (data->y < -MIN_JOYSTICK_MOVED && data->stickState != SSDown)
            {
                data->stickState = SSMovedDown;
            }
        }
    }

    data->switchState = (SwitchState)(data->switchState & ~JOYSTICK_PRESS_MASK);

    if (!digitalRead(swPin))
    {
        if (data->switchState != SSHeld)
        {
            data->switchState = SSPressed;
        }
    }
    else if (data->switchState != SSIdle)
    {
        data->switchState = SSReleased;
    }
}