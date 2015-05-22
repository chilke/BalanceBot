#ifndef RemoteMenu_h
#define RemoteMenu_h

#include "Arduino.h"
#include "Joystick.h"
#include "ControlState.h"

enum MenuSelection: uint8_t
{
    MSControlFormat=0,
    MSKp=1,
    MSKd=2,
    MSKi=3,
    MSTrim=4,
    MSIdleTimeout=5,
    MSPowerTimeout=6,
    MSResetRemote=7,
    MSResetRobot=8,
    MSMinSpeed=9,
    MSMaxSpeed=10,
    MSMaxTurn=11,
    MSLeanCount=12,
    MSEnd=13,
};

enum MenuState: uint8_t
{
    MSMenu=0,
    MSDetail=1,
    MSReset=2,
};

class RM
{
public:
    void begin();
    void reenter();
    bool handleInput(JoystickData *leftState, JoystickData *rightState);
    void updateValue(float value);
    void updateValue(int16_t value);
    void updateValue(ControlStateFormat value);
private:
    void updateDisplay();
    void enterMenuMode();
    void enterDetailMode();


    bool needsValue;
    MenuSelection currentSelection;
    MenuState currentState;
    uint8_t repeatCount;
};

#endif