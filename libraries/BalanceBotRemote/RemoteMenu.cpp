#include "RemoteMenu.h"
#include "RemoteController.h"

void RM::begin()
{
    currentSelection = (MenuSelection)0;
    needsValue = false;
    enterMenuMode();
}

void RM::reenter()
{
    if (currentState == MSMenu)
    {
        enterMenuMode();
    }
    else
    {
        enterDetailMode();
    }
}

bool RM::handleInput(JoystickData *leftState, JoystickData *rightState)
{
    int8_t adder = 0;
    if (currentState == MSMenu)
    {
        if (leftState->switchState == SSPressed)
        {
            return false;
        }

        if (rightState->switchState == SSPressed)
        {
            enterDetailMode();
            return true;
        }

        if (leftState->stickState == SSMovedDown)
        {

            currentSelection = (MenuSelection)(currentSelection + 1);
            if (currentSelection >= MSEnd)
            {
                currentSelection = (MenuSelection)0;
            }
            updateDisplay();
        }
        else if (leftState->stickState == SSMovedUp)
        {
            currentSelection = (MenuSelection)(currentSelection - 1);
            if (currentSelection >= MSEnd)
            {
                currentSelection = (MenuSelection)(MSEnd - 1);
            }
            updateDisplay();
        }
        else if (needsValue)
        {
            updateDisplay();
        }
    }
    else
    {
        if (leftState->switchState == SSPressed)
        {
            switch (currentSelection)
            {
                case MSControlFormat:
                case MSIdleTimeout:
                case MSPowerTimeout:
                RemoteController.cancelRemoteValue();
                break;
                case MSResetRemote:
                case MSResetRobot:
                break;
                default:
                RemoteController.cancelRobotValue();
                break;
            }
            enterMenuMode();
            return true;
        }

        if (rightState->switchState == SSPressed)
        {
            switch (currentSelection)
            {
                case MSControlFormat:
                case MSIdleTimeout:
                case MSPowerTimeout:
                RemoteController.commitRemoteValue();
                break;
                case MSResetRemote:
                RemoteController.resetRemoteDefaults();
                break;
                case MSResetRobot:
                RemoteController.resetRobotDefaults();
                break;
                default:
                RemoteController.commitRobotValue();
                break;
            }
            enterMenuMode();
            return true;
        }

        if (leftState->stickState == SSMovedUp ||
            leftState->stickState == SSUp)
        {
            if (leftState->stickState == SSMovedUp)
            {
                repeatCount = 0;
                adder = 1;
            }
            else if (repeatCount % 5 == 0)
            {
                if (repeatCount < 10)
                {
                    adder = 2;
                }
                else if (repeatCount < 20)
                {
                    adder = 4;
                }
                else if (repeatCount < 30)
                {
                    adder = 8;
                }
                else {
                    adder = 16;
                }
            }
            repeatCount++;
        }
        else if (leftState->stickState == SSMovedDown ||
            leftState->stickState == SSDown)
        {
            if (leftState->stickState == SSMovedDown)
            {
                repeatCount = 0;
                adder = -1;
            }
            else if (repeatCount % 5 == 0)
            {
                if (repeatCount < 10)
                {
                    adder = -2;
                }
                else if (repeatCount < 20)
                {
                    adder = -4;
                }
                else if (repeatCount < 30)
                {
                    adder = -8;
                }
                else {
                    adder = -16;
                }
            }
            repeatCount++;
        }

        if (adder != 0)
        {
            switch (currentSelection)
            {
                case MSMinSpeed:
                RemoteController.updateRobotValue(RVMinSpeed, adder);
                break;
                case MSMaxSpeed:
                RemoteController.updateRobotValue(RVMaxSpeed, adder);
                break;
                case MSMaxTurn:
                RemoteController.updateRobotValue(RVMaxTurn, adder);
                break;
                case MSMotorKp:
                RemoteController.updateRobotValue(RVMotorKp, adder);
                break;
                case MSMotorKd:
                RemoteController.updateRobotValue(RVMotorKd, adder);
                break;
                case MSMotorKi:
                RemoteController.updateRobotValue(RVMotorKi, adder);
                break;
                case MSBalanceKp:
                RemoteController.updateRobotValue(RVBalanceKp, adder);
                break;
                case MSBalanceKd:
                RemoteController.updateRobotValue(RVBalanceKd, adder);
                break;
                case MSBalanceKi:
                RemoteController.updateRobotValue(RVBalanceKi, adder);
                break;
                case MSControlFormat:
                RemoteController.updateRemoteValue(RVControlFormat, adder);
                break;
                case MSIdleTimeout:
                RemoteController.updateRemoteValue(RVIdleTimeout, adder);
                break;
                case MSPowerTimeout:
                RemoteController.updateRemoteValue(RVPowerTimeout, adder);
                break;
            }

            needsValue = true;
        }
        else if (needsValue)
        {
            updateDisplay();
        }
    }
    return true;
}

void RM::updateDisplay()
{
    RemoteController.lcd.setCursor(0,0);
    switch (currentSelection)
    {
        case MSMaxTurn:
        RemoteController.lcd.print(F("MaxTurn       NC"));
        RemoteController.updateRobotValue(RVMaxTurn, 0);
        break;
        case MSMaxSpeed:
        RemoteController.lcd.print(F("MaxSpeed      NC"));
        RemoteController.updateRobotValue(RVMaxSpeed, 0);
        break;
        case MSMinSpeed:
        RemoteController.lcd.print(F("MinSpeed      NC"));
        RemoteController.updateRobotValue(RVMinSpeed, 0);
        break;
        case MSMotorKp:
        RemoteController.lcd.print(F("Motor Kp      NC"));
        RemoteController.updateRobotValue(RVMotorKp, 0);
        break;
        case MSMotorKd:
        RemoteController.lcd.print(F("Motor Kd      NC"));
        RemoteController.updateRobotValue(RVMotorKd, 0);
        break;
        case MSMotorKi:
        RemoteController.lcd.print(F("Motor Ki      NC"));
        RemoteController.updateRobotValue(RVMotorKi, 0);
        break;
        case MSBalanceKp:
        RemoteController.lcd.print(F("BalanceKp     NC"));
        RemoteController.updateRobotValue(RVBalanceKp, 0);
        break;
        case MSBalanceKd:
        RemoteController.lcd.print(F("BalanceKd     NC"));
        RemoteController.updateRobotValue(RVBalanceKd, 0);
        break;
        case MSBalanceKi:
        RemoteController.lcd.print(F("BalanceKi     NC"));
        RemoteController.updateRobotValue(RVBalanceKi, 0);
        break;
        case MSControlFormat:
        RemoteController.lcd.print(F("Control "));
        RemoteController.updateRemoteValue(RVControlFormat, 0);
        break;
        case MSIdleTimeout:
        RemoteController.lcd.print(F("Idle Time      "));
        RemoteController.updateRemoteValue(RVIdleTimeout, 0);
        break;
        case MSPowerTimeout:
        RemoteController.lcd.print(F("Power Time     "));
        RemoteController.updateRemoteValue(RVPowerTimeout, 0);
        break;
        case MSResetRemote:
        RemoteController.lcd.print(F("Reset Rem Values"));
        break;
        case MSResetRobot:
        RemoteController.lcd.print(F("Reset Bot Values"));
        break;
    }

    needsValue = true;
}

void RM::updateValue(float value)
{
    uint8_t digits = 4;
    float compare = 10;

    RemoteController.lcd.setCursor(9, 0);

    if (value >= 0)
    {
        RemoteController.lcd.print(' ');
    }

    while (abs(value) >= compare)
    {
        if (digits == 0)
        {
            Serial.println(F("Uh-oh, updateValue overrun"));
        }
        digits--;
        compare *= 10;
    }

    RemoteController.lcd.print(value, digits);

    needsValue = false;
}

void RM::updateValue(int16_t value)
{
    RemoteController.lcd.setCursor(10, 0);

    if (value >= 0)
    {
        RemoteController.lcd.print(' ');
    }

    if (abs(value) < 10000)
    {
        RemoteController.lcd.print(' ');

        if (abs(value) < 1000)
        {
            RemoteController.lcd.print(' ');

            if (abs(value) < 100)
            {
                RemoteController.lcd.print(' ');

                if (abs(value) < 10)
                {
                    RemoteController.lcd.print(' ');
                }
            }
        }
    }

    RemoteController.lcd.print(value);

    needsValue = false;
}

void RM::updateValue(ControlStateFormat value)
{
    RemoteController.lcd.setCursor(8, 0);
    switch (value)
    {
        case FormatOneStickLeft:
        RemoteController.lcd.print(F("    Left"));
        break;
        case FormatOneStickRight:
        RemoteController.lcd.print(F("   Right"));
        break;
        case FormatTwoStick:
        RemoteController.lcd.print(F("    Tank"));
        break;
        case FormatRCLeft:
        RemoteController.lcd.print(F(" RC Left"));
        break;
        case FormatRCRight:
        RemoteController.lcd.print(F("RC Right"));
        break;
    }

    needsValue = false;
}

void RM::enterMenuMode()
{
    currentState = MSMenu;
    RemoteController.lcd.setCursor(0, 1);
    RemoteController.lcd.print(F("<Exit      Edit>"));
    updateDisplay();
}

void RM::enterDetailMode()
{
    currentState = MSDetail;
    RemoteController.lcd.setCursor(0, 1);
    RemoteController.lcd.print(F("<Cancel    Save>"));
    updateDisplay();
}