#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"

#define MAX_JOYSTICK_ZEROED 30
#define MIN_JOYSTICK_MOVED 50
#define MIN_JOYSTICK_DIFF 10
#define JOYSTICK_MOVED_MASK 0x01
#define JOYSTICK_PRESS_MASK 0x01

enum SwitchState: uint8_t
{
    SSIdle=0,
    SSReleased=1,
    SSHeld=2,
    SSPressed=3,
};

enum StickState: uint8_t
{
    SSZeroed=0,
    SSInvalid=1,
    SSUp=2,
    SSMovedUp=3,
    SSDown=4,
    SSMovedDown=5,
    SSLeft=6,
    SSMovedLeft=7,
    SSRight=8,
    SSMovedRight=9,
};

struct JoystickData
{
    int8_t x;
    int8_t y;
    StickState stickState;
    SwitchState switchState;
    bool isIdle() {
        return (x == 0 && y == 0 && switchState == SSIdle);
    }
};

struct JoystickCalibrationData
{
    uint8_t deadZone;
    int16_t zeroX;
    int16_t zeroY;
    int16_t maxX;
    int16_t minX;
    int16_t maxY;
    int16_t minY;
    void debug() {
        Serial.print("dz="); Serial.print(deadZone);
        Serial.print("  zX="); Serial.print(zeroX);
        Serial.print("  zY="); Serial.print(zeroY);
        Serial.print("  <X="); Serial.print(minX);
        Serial.print("  >X="); Serial.print(maxX);
        Serial.print("  <Y="); Serial.print(minY);
        Serial.print("  >Y="); Serial.println(maxY);
    }
};

class Joystick
{
public:
    Joystick(uint8_t xPin, uint8_t yPin, uint8_t swPin);
    void setCalibrationData(JoystickCalibrationData *calData);
    void read(JoystickData *data);
    void begin();
private:
    uint8_t xPin;
    uint8_t yPin;
    uint8_t swPin;
    JoystickCalibrationData *calibrationData;
};

#endif