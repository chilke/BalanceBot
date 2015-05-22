#ifndef RemoteController_h
#define RemoteController_h

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "BotDefines.h"
#include "Joystick.h"
#include "RemoteCommunication.h"
#include "RemoteMenu.h"

enum RCState
{
    RCSNormal,
    RCSMenu,
    RCSIdle,
    RCSHeld,
};

class RC
{
public:
	RC();
	void begin();
	void doWork();
	void handleCommCommand(CommunicationCommand command, uint8_t *data, uint8_t length);
    void updateRobotValue(RobotValue value, int8_t change);
    void updateRemoteValue(RemoteValue value, int8_t change);
    void cancelRobotValue();
    void cancelRemoteValue();
    void commitRobotValue();
    void commitRemoteValue();
    void resetRobotDefaults();
    void resetRemoteDefaults();

	LiquidCrystal_I2C lcd;

private:
    RCState currentState;
    RCState lastState;

    JoystickData leftState;
    JoystickData rightState;
    Joystick leftStick;
    Joystick rightStick;
    RM remoteMenu;
    CommunicationCommand queuedCommand;

    uint32_t lastTime;
    uint8_t failedComm;

    uint8_t idleTicks;
    uint16_t idleSecs;

    uint16_t batteryCount;

    RemoteCommunication comm;

    void refreshNormalDisplay();
    void doShutdown();

    void startCountdown();
    void refreshCountdown(uint16_t count);

    void updateValue(int16_t *value, int16_t min, int16_t max, int8_t delta);

    void printValue(int8_t value);
};

extern RC RemoteController;

#endif