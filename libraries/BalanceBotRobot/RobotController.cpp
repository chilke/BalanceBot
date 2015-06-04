#include "Robot.h"

RC::RC() : leftMotor(3, 4, 9), rightMotor(5, 6, 10),
accelGyro(0x68), comm(COMMUNICATION_CE, COMMUNICATION_CS),
motorPid(&RobotStorage.data.values.kp,
    &RobotStorage.data.values.ki,
    &RobotStorage.data.values.kd)
{
}

void RC::begin()
{
    RobotStorage.begin(false);

    Wire.begin();
    TWBR = 24;

    accelGyro.initialize();
    accelGyro.setFullScaleGyroRange(GYRO_RANGE);

    accelGyro.setXAccelOffset(X_ACCEL_OFF);
    accelGyro.setYAccelOffset(Y_ACCEL_OFF);
    accelGyro.setZAccelOffset(Z_ACCEL_OFF);

    accelGyro.setXGyroOffset(X_GYRO_OFF);
    accelGyro.setYGyroOffset(Y_GYRO_OFF);
    accelGyro.setZGyroOffset(Z_GYRO_OFF);

    f_angle = 0;
    trim = 0;
    lean = 0;
    currentSpeed = 0;

    leftTurn = 0;
    rightTurn = 0;

    comm.begin();

    lastTime = millis();
    lastControlTime = lastTime;
}

void RC::handleControls(ControlState *state)
{
    if (state->format == FormatOne)
    {
        currentSpeed = state->format1.speed * RobotStorage.data.values.maxSpeed;
        leftTurn = state->format1.turn * RobotStorage.data.values.maxTurn;
        rightTurn = -leftTurn;
    }

    if (currentSpeed != 0 || leftTurn != 0)
    {
        lastControlTime = millis();
    }
}

void RC::handleCommCommand(CommunicationCommand command, uint8_t *data, uint8_t length)
{
    CommunicationCommand responseCommand = CCGetBattery;
    uint8_t *responseData = (uint8_t *)&batteryLevel;
    uint8_t responseLength = sizeof(batteryLevel);

 //   Serial.print("Received command "); Serial.println((uint16_t)command, HEX);

    switch (command)
    {
        case CCControls:
        if (length == sizeof(ControlState))
        {
            handleControls((ControlState *)data);
        }
        break;
        case CCSetReadingPipe:
        if (length == sizeof(RobotStorage.data.values.readingPipe))
        {
            RobotStorage.data.values.readingPipe = *(uint64_t *)data;
        }
        break;
        case CCSetWritingPipe:
        if (length == sizeof(RobotStorage.data.values.writingPipe))
        {
            RobotStorage.data.values.writingPipe = *(uint64_t *)data;
        }
        break;
        case CCUpdateKp:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.kp, KP_MIN, KP_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateKp;
        responseData = (uint8_t *)&RobotStorage.data.values.kp;
        responseLength = sizeof(RobotStorage.data.values.kp);
        break;
        case CCUpdateKi:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.ki, KI_MIN, KI_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateKi;
        responseData = (uint8_t *)&RobotStorage.data.values.ki;
        responseLength = sizeof(RobotStorage.data.values.ki);
        break;
        case CCUpdateKd:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.kd, KD_MIN, KD_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateKd;
        responseData = (uint8_t *)&RobotStorage.data.values.kd;
        responseLength = sizeof(RobotStorage.data.values.kd);
        break;
        case CCUpdateTrim:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.trim, TRIM_ADDER_MIN, TRIM_ADDER_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateTrim;
        responseData = (uint8_t *)&RobotStorage.data.values.trim;
        responseLength = sizeof(RobotStorage.data.values.trim);
        break;
        case CCSetChannel:
        if (length == 1)
        {
            RobotStorage.data.values.channel = *(uint8_t *)data;
        }
        break;
        case CCCommit:
        if (length == 0)
        {
            RobotStorage.commit();
        }
        break;
        case CCRestore:
        if (length == 0)
        {
            RobotStorage.restore();
        }
        break;
        case CCResetDefaults:
        if (length == 0)
        {
            RobotStorage.resetDefaults();
            RobotStorage.commit();
        }
        break;
        case CCGetBattery:
        break;
        case CCUpdateMinSpeed:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.minSpeed,
                MIN_MOTOR_SPEED_MIN, MIN_MOTOR_SPEED_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateMinSpeed;
        responseData = (uint8_t *)&RobotStorage.data.values.minSpeed;
        responseLength = sizeof(RobotStorage.data.values.minSpeed);
        break;
        case CCUpdateMaxSpeed:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.maxSpeed,
                MAX_SPEED_MIN, MAX_SPEED_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateMaxSpeed;
        responseData = (uint8_t *)&RobotStorage.data.values.maxSpeed;
        responseLength = sizeof(RobotStorage.data.values.maxSpeed);
        break;
        case CCUpdateMaxTurn:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.maxTurn,
                MAX_TURN_MIN, MAX_TURN_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateMaxTurn;
        responseData = (uint8_t *)&RobotStorage.data.values.maxTurn;
        responseLength = sizeof(RobotStorage.data.values.maxTurn);
        break;
        case CCUpdateLeanCount:
        if (length == 1)
        {
            updateValue(&RobotStorage.data.values.leanCount,
                LEAN_COUNT_MIN, LEAN_COUNT_MAX, *(int8_t *)data);
        }
        responseCommand = CCUpdateLeanCount;
        responseData = (uint8_t *)&RobotStorage.data.values.leanCount;
        responseLength = sizeof(RobotStorage.data.values.leanCount);
        break;
        default:
        responseCommand = CCNack;
    }

    comm.sendCommand(responseCommand, responseData, responseLength);
}

void RC::doWork()
{
    int16_t ax, ay, az, gx;
    int16_t leftOut = 0;
    int16_t rightOut = 0;
    float r_angle;
    float error;
    float K, A, omega;

    float dt = (millis()-lastTime) / 1000.0;
    lastTime = millis();

    calculateBatteryLevel();

    accelGyro.getMotion6(&ax, &ay, &az, &gx, &ax, &ax);

    r_angle = atan2(ay, az) * 180 / PI;
    omega = gx / GYRO_SENSITIVITY;

    K = 0.8;
    A = K / (K + dt);
    f_angle = A * (f_angle + omega * dt) + (1-A) * r_angle;
    if (abs(r_angle) < MAX_STABLE_ANGLE)
    {
        error = trim + currentSpeed - f_angle;

        error = motorPid.process(error);

        if (millis()-lastControlTime > MAX_IDLE_CONTROLS)
        {
            currentSpeed = 0;
            //TODO turns = 0
            if (error > 0)
            {
                lean++;
                if (lean > RobotStorage.data.values.leanCount)
                {
                    lean = 0;
                    trim += RobotStorage.data.values.trim;
                }
            }
            else if (error < 0)
            {
                lean--;
                if (lean < -RobotStorage.data.values.leanCount)
                {
                    lean = 0;
                    trim -= RobotStorage.data.values.trim;
                }
            }
        }

        leftOut = error;
        leftOut += leftTurn;
        rightOut = error;
        rightOut += rightTurn;
    }
    else
    {
        motorPid.reset();
        leftOut = rightOut = 0;
    }

    leftMotor.setVelocity(leftOut);
    rightMotor.setVelocity(rightOut);

    comm.checkReceive();

    RobotStorage.doWork();
}

void RC::calculateBatteryLevel()
{
    uint16_t raw = analogRead(BATTERY_PIN);
    if (raw < BATTERY_MIN)
    {
        return;
    }
    raw -= BATTERY_MIN;
    raw *= 100;
    raw /= BATTERY_MAX;
    batteryLevel = (uint8_t)raw;
}

void RC::updateValue(float *value, float min, float max, int8_t delta)
{
    *value += (max - min) / 500 * delta;
    if (*value > max)
    {
        *value = max;
    }
    else if (*value < min)
    {
        *value = min;
    }
}

void RC::updateValue(uint8_t *value, uint8_t min, uint8_t max, int8_t delta)
{
    if (delta < 0)
    {
        uint8_t newValue = *value + delta;

        if (newValue > *value)
        {
            *value = 0;
        }
        else
        {
            *value = newValue;
        }

        if (*value < min)
        {
            *value = min;
        }
    }
    else
    {
        if (max - *value > delta)
        {
            *value += delta;
        }
        else
        {
            *value = max;
        }
    }
}

RC RobotController;