#ifndef RemoteMenu_h
#define RemoteMenu_h

#include "Arduino.h"
#include "Joystick.h"
#include "ControlState.h"

enum MenuSelection: uint8_t
{
    MSControlFormat,
    MSMotorKp,
    MSMotorKd,
    MSMotorKi,
    MSBalanceKp,
    MSBalanceKd,
    MSBalanceKi,
    MSIdleTimeout,
    MSPowerTimeout,
    MSResetRemote,
    MSResetRobot,
    MSMinSpeed,
    MSMaxSpeed,
    MSMaxTurn,
    MSEnd,
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