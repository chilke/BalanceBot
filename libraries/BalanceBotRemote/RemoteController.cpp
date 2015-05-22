#include "Remote.h"

RC::RC(): lcd(LCD_ADDR, LCD_EN, LCD_RW, LCD_RS,
	LCD_D4, LCD_D5, LCD_D6, LCD_D7, LCD_BL, LCD_BL_POLARITY),
leftStick(JOYSTICK_LEFT_X_PIN, JOYSTICK_LEFT_Y_PIN, JOYSTICK_LEFT_SW_PIN),
rightStick(JOYSTICK_RIGHT_X_PIN, JOYSTICK_RIGHT_Y_PIN, JOYSTICK_RIGHT_SW_PIN),
comm(COMMUNICATION_CE, COMMUNICATION_CS)
{
}

void RC::begin()
{
    pinMode(REMOTE_POWER_PIN, OUTPUT);
    digitalWrite(REMOTE_POWER_PIN, REMOTE_POWER_PIN_ON);

    lcd.begin(LCD_COLUMNS, LCD_ROWS);
    lcd.backlight();

    RemoteStorage.begin(false);

    leftStick.begin();
    leftStick.setCalibrationData(&RemoteStorage.data.values.leftCalibration);
    rightStick.begin();
    rightStick.setCalibrationData(&RemoteStorage.data.values.rightCalibration);

    memset(&leftState, 0x00, sizeof(leftState));
    memset(&rightState, 0x00, sizeof(rightState));
    leftState.switchState = SSHeld;
    rightState.switchState = SSHeld;

    comm.begin();

    refreshNormalDisplay();

    currentState = RCSNormal;
    lastState = RCSNormal;
    queuedCommand = CCGetBattery;
    failedComm = 0;
    batteryCount = BATTERY_REFRESH;
    lastTime = millis();
}

void RC::updateRemoteValue(RemoteValue value, int8_t change)
{
    switch(value)
    {
        case RVControlFormat:
        if (change > 0)
        {
            RemoteStorage.data.values.controlFormat = 
            (ControlStateFormat)((uint8_t)RemoteStorage.data.values.controlFormat
                + 1);
            if (RemoteStorage.data.values.controlFormat >= FormatEnd)
            {
                RemoteStorage.data.values.controlFormat = (ControlStateFormat)0;
            }
        }
        else if (change < 0)
        {
            RemoteStorage.data.values.controlFormat = 
            (ControlStateFormat)((uint8_t)RemoteStorage.data.values.controlFormat
                - 1);
            if (RemoteStorage.data.values.controlFormat >= FormatEnd)
            {
                RemoteStorage.data.values.controlFormat = (ControlStateFormat)(FormatEnd-1);
            }
        }
        remoteMenu.updateValue(RemoteStorage.data.values.controlFormat);
        break;
        case RVIdleTimeout:
        updateValue(&RemoteStorage.data.values.idleTimeout,
            IDLE_TIMEOUT_MIN, IDLE_TIMEOUT_MAX, change);
        remoteMenu.updateValue(RemoteStorage.data.values.idleTimeout);
        break;
        case RVPowerTimeout:
        updateValue(&RemoteStorage.data.values.powerTimeout,
            POWER_TIMEOUT_MIN, POWER_TIMEOUT_MAX, change);
        remoteMenu.updateValue(RemoteStorage.data.values.powerTimeout);
        break;
    }
}

void RC::updateRobotValue(RobotValue value, int8_t change)
{
    switch (value)
    {
        case RVLeanCount:
        queuedCommand = CCUpdateLeanCount;
        break;
        case RVMinSpeed:
        queuedCommand = CCUpdateMinSpeed;
        break;
        case RVMaxSpeed:
        queuedCommand = CCUpdateMaxSpeed;
        break;
        case RVMaxTurn:
        queuedCommand = CCUpdateMaxTurn;
        break;
        case RVKp:
        queuedCommand = CCUpdateKp;
        break;
        case RVKd:
        queuedCommand = CCUpdateKd;
        break;
        case RVKi:
        queuedCommand = CCUpdateKi;
        break;
        case RVTrim:
        queuedCommand = CCUpdateTrim;
        break;
    }

    if (!comm.isWaitingResponse)
    {
        comm.sendCommand(queuedCommand, (uint8_t *)&change, sizeof(change));
        queuedCommand = CCGetBattery;
    }
}

void RC::cancelRobotValue()
{
    if (!comm.isWaitingResponse)
    {
        comm.sendCommand(CCRestore, NULL, 0);
    }
    else
    {
        Serial.println("Uh-oh, trying to cancel, but comm is busy");
    }
}

void RC::cancelRemoteValue()
{
    RemoteStorage.restore();
}

void RC::commitRobotValue()
{
    if (!comm.isWaitingResponse)
    {
        comm.sendCommand(CCCommit, NULL, 0);
    }
    else
    {
        Serial.println("Uh-oh, trying to commit, but comm is busy");
    }
}

void RC::commitRemoteValue()
{
    RemoteStorage.commit();
}

void RC::resetRobotDefaults()
{
    if (!comm.isWaitingResponse)
    {
        comm.sendCommand(CCResetDefaults, NULL, 0);
    }
    else
    {
        Serial.println("Uh-oh, trying to reset, but comm is busy");
    }
}

void RC::resetRemoteDefaults()
{
    RemoteStorage.resetDefaults();
    RemoteStorage.commit();
}

void RC::doWork()
{
    ControlState cs;
    uint16_t remainingTime;
    comm.checkReceive();
    if (millis() - lastTime > REMOTE_MILLIS_PER_TICK)
    {
        lastTime = millis();

        if (batteryCount < BATTERY_REFRESH)
        {
            batteryCount++;
        }

        if (comm.isWaitingResponse)
        {
            failedComm++;
            if (comm.isConnected && failedComm >= 5)
            {
                comm.isConnected = false;
                if (currentState == RCSNormal)
                {
                    refreshNormalDisplay();
                }
            }
            comm.isWaitingResponse = false;
        }

        leftStick.read(&leftState);
        rightStick.read(&rightState);

        if (leftState.isIdle() && rightState.isIdle())
        {
            idleTicks++;
            if (idleTicks >= TICKS_PER_SEC)
            {
                idleTicks = 0;
                idleSecs++;
                if (idleSecs >= RemoteStorage.data.values.idleTimeout)
                {
                    remainingTime = idleSecs - RemoteStorage.data.values.idleTimeout;
                    if (currentState != RCSIdle)
                    {
                        lastState = currentState;
                        currentState = RCSIdle;
                        lcd.setCursor(0, 0);
                        lcd.print(F(" Idle shut down "));
                        startCountdown();
                    }

                    if (remainingTime >= RemoteStorage.data.values.powerTimeout)
                    {
                        doShutdown();
                    }

                    remainingTime = RemoteStorage.data.values.powerTimeout - remainingTime;

                    refreshCountdown(remainingTime);
                }
            }
        }
        else if (leftState.switchState == SSHeld)
        {
            idleTicks++;
            if (idleTicks >= TICKS_PER_SEC)
            {
                idleTicks = 0;
                idleSecs++;
                
                if (idleSecs == 1)
                {
                    lastState = currentState;
                    currentState = RCSHeld;
                    lcd.setCursor(0, 0);
                    lcd.print(F("Manual shut down"));
                    startCountdown();
                }

                if (idleSecs >= REMOTE_MANUAL_POWER_TIMEOUT)
                {
                    doShutdown();
                }

                refreshCountdown(REMOTE_MANUAL_POWER_TIMEOUT - idleSecs);
            }
        }
        else
        {
            if (currentState != RCSNormal && currentState != RCSMenu)
            {
                currentState = lastState;
                leftStick.read(&leftState);
                rightStick.read(&rightState);
                if (currentState == RCSNormal)
                {
                    refreshNormalDisplay();
                }
                else
                {
                    remoteMenu.reenter();
                }
            }
            idleTicks = 0;
            idleSecs = 0;
        }

        if (currentState == RCSMenu)
        {
            if (!remoteMenu.handleInput(&leftState, &rightState))
            {
                currentState = RCSNormal;
                refreshNormalDisplay();
            }
        }
        else if (currentState == RCSNormal)
        {
            if (rightState.switchState == SSPressed)
            {
                remoteMenu.begin();
                currentState = RCSMenu;
            }
            else if (comm.isConnected)
            {
                switch (RemoteStorage.data.values.controlFormat)
                {
                    case FormatOneStickLeft:
                    cs.format1.format = FormatOne;
                    cs.format1.speed = leftState.y;
                    cs.format1.turn = leftState.x;
                    break;
                    case FormatOneStickRight:
                    cs.format1.format = FormatOne;
                    cs.format1.speed = rightState.y;
                    cs.format1.turn = rightState.x;
                    break;
                    case FormatTwoStick:
                    cs.format2.format = FormatOne;
                    cs.format2.left = leftState.y;
                    cs.format2.right = rightState.y;
                    break;
                    case FormatRCLeft:
                    cs.format1.format = FormatOne;
                    cs.format1.speed = leftState.y;
                    cs.format1.turn = rightState.x;
                    break;
                    case FormatRCRight:
                    cs.format1.format = FormatOne;
                    cs.format1.speed = rightState.y;
                    cs.format1.turn = leftState.x;
                    break;
                }

                comm.sendCommand(CCControls,
                    (uint8_t *)&cs, sizeof(cs));
            }
        }

        if (!comm.isWaitingResponse)
        {
            comm.sendCommand(queuedCommand, NULL, 0);
            queuedCommand = CCGetBattery;
        }
    }

    RemoteStorage.doWork();
}

void RC::startCountdown()
{
    lcd.setCursor(0, 1);
    lcd.print(F(" in     seconds "));
}

void RC::refreshCountdown(uint16_t count)
{
    if (count == 1)
    {
        lcd.setCursor(14, 1);
        lcd.print(' ');
    }

    lcd.setCursor(4, 1);

    if (count < 100)
    {
        lcd.print(' ');

        if (count < 10)
        {
            lcd.print(' ');
        }
    }

    lcd.print(count);
}

void RC::handleCommCommand(CommunicationCommand command, uint8_t *data, uint8_t length)
{
    if (!comm.isConnected)
    {
        comm.isConnected = true;
        if (currentState == RCSNormal)
        {
            refreshNormalDisplay();
        }
    }
    switch(command)
    {
        case CCUpdateMaxTurn:
        case CCUpdateMaxSpeed:
        case CCUpdateMinSpeed:
        case CCUpdateKp:
        case CCUpdateKi:
        case CCUpdateKd:
        case CCUpdateTrim:
        remoteMenu.updateValue(*(float *)data);
        break;
        case CCUpdateLeanCount:
        remoteMenu.updateValue(*(uint8_t *)data);
        break;
        case CCGetBattery:
        if (currentState == RCSNormal && batteryCount >= BATTERY_REFRESH)
        {
            lcd.setCursor(11, 0);
            uint8_t battery = *data;
            if (battery < 100)
            {
                lcd.print(' ');

                if (battery < 10)
                {
                    lcd.print(' ');
                }
            }
            lcd.print(battery);
            batteryCount = 0;
        }
        break;
    }
    failedComm = 0;
    comm.isWaitingResponse = false;
}

void RC::doShutdown()
{
    digitalWrite(REMOTE_POWER_PIN, REMOTE_POWER_PIN_OFF);
    lcd.setCursor(0, 0);
    lcd.print("Release buttons ");
    lcd.setCursor(0, 1);
    lcd.print(" to power down  ");
    exit(1);
}

void RC::refreshNormalDisplay()
{
    lcd.setCursor(0, 0);
    
    if (comm.isConnected)
    {
        lcd.print(F(" Connected    % "));
        batteryCount = BATTERY_REFRESH;
    }
    else
    {
        lcd.print(F(" Not Connected  "));
    }
    
    lcd.setCursor(0, 1);
    lcd.print(F("           Menu>"));
}

void RC::updateValue(int16_t *value, int16_t min, int16_t max, int8_t delta)
{
    *value += delta;
    if (*value > max)
    {
        *value = max;
    }
    else if (*value < min)
    {
        *value = min;
    }
}

RC RemoteController;