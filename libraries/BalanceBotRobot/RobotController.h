#ifndef RobotController_h
#define RobotController_h

#include "Arduino.h"
#include "BotDefines.h"
#include "RobotCommunication.h"
#include "ControlState.h"
#include "Motor.h"
#include "MPU6050.h"
#include "PidControl.h"

class RC
{
public:
    RC();
    void begin();
    void doWork();
    void handleCommCommand(CommunicationCommand command, uint8_t *data, uint8_t length);
    void handleControls(ControlState *state);
private:
    void calculateBatteryLevel();
    void updateValue(float *value, float min, float max, int8_t delta);
    void updateValue(uint8_t *value, uint8_t min, uint8_t max, int8_t delta);

    Motor leftMotor;
    Motor rightMotor;
    MPU6050 accelGyro;
    RobotCommunication comm;
    PidControl motorPid;
    float f_angle;

    float trim;
    float balancedTrim;
    uint8_t balancedCount;

    int8_t lean;

    uint8_t batteryLevel;

    float currentSpeed;

    int8_t leftTurn;
    int8_t rightTurn;

    uint32_t lastTime;
    uint32_t lastControlTime;
};

extern RC RobotController;

#endif