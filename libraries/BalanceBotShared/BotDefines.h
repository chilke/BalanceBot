#ifndef BotDefines_h
#define BotDefines_h

#include "Arduino.h"

//#define DEBUG

#define X_ACCEL_OFF -1111
#define Y_ACCEL_OFF -643
#define Z_ACCEL_OFF 1363
#define X_GYRO_OFF 70
#define Y_GYRO_OFF 61
#define Z_GYRO_OFF -9

#define GYRO_RANGE MPU6050_GYRO_FS_2000
#define GYRO_SENSITIVITY 16.384


#define MIN_MOTOR_SPEED 0
#define MAX_MOTOR_SPEED 255

#define MIN_BALANCED_COUNT 100

#define MAX_IDLE_CONTROLS 1000
#define MAX_INTEGRAL 20000
#define MAX_STABLE_ANGLE 30

#define PI 3.14159
#define PI_4 0.7854
#define PI_DEGREES 180

/* Hardware ---------------------------------------------*/
#define COMMUNICATION_CE 8
#define COMMUNICATION_CS 7

#define BATTERY_PIN A2
#define BATTERY_MIN 716 //11.2V / 3.199 / 5V * 1023
#define BATTERY_MAX 70 //12.3V / 3.199 / 5 * 1023 - BATTERY_MIN

#define BATTERY_REFRESH 600 //30 seconds * 20 ticks per second

#define REMOTE_POWER_PIN 4
#define REMOTE_POWER_PIN_ON 1
#define REMOTE_POWER_PIN_OFF 0

#define REMOTE_MILLIS_PER_TICK 50
//#define REMOTE_MILLIS_PER_TICK 1000
#define TICKS_PER_SEC 20
#define REMOTE_IDLE_TIMEOUT_DEFAULT 300
#define REMOTE_POWER_TIMEOUT_DEFAULT 15
#define REMOTE_MANUAL_POWER_TIMEOUT 5

#define IDLE_TIMEOUT_MIN 15
#define IDLE_TIMEOUT_MAX 900
#define POWER_TIMEOUT_MIN 5
#define POWER_TIMEOUT_MAX 120

/* Communication ----------------------------------------*/
enum RobotValue: uint8_t
{
    RVMotorKp,
    RVMotorKd,
    RVMotorKi,
    RVBalanceKp,
    RVBalanceKd,
    RVBalanceKi,
    RVMinSpeed,
    RVMaxSpeed,
    RVMaxTurn,
};

enum RemoteValue: uint8_t
{
    RVControlFormat,
    RVIdleTimeout,
    RVPowerTimeout,
};

/* LCD --------------------------------------------------*/
#define LCD_ADDR 0x27
#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BL 3
#define LCD_BL_POLARITY POSITIVE
#define LCD_COLUMNS 16
#define LCD_ROWS 2

/* Joysticks --------------------------------------------*/
#define JOYSTICK_LEFT_X_PIN A1
#define JOYSTICK_LEFT_Y_PIN A0
#define JOYSTICK_LEFT_SW_PIN 2
#define JOYSTICK_RIGHT_X_PIN A3
#define JOYSTICK_RIGHT_Y_PIN A2
#define JOYSTICK_RIGHT_SW_PIN 3

/* Storage ----------------------------------------------*/
#define DEFAULT_PIPE_0 0xF0F0F0F0E1LL
#define DEFAULT_PIPE_1 0xF0F0F0F0D2LL

#define ROBOT_READING_PIPE_DEFAULT DEFAULT_PIPE_0
#define ROBOT_WRITING_PIPE_DEFAULT DEFAULT_PIPE_1
#define REMOTE_READING_PIPE_DEFAULT DEFAULT_PIPE_1
#define REMOTE_WRITING_PIPE_DEFAULT DEFAULT_PIPE_0

#define MOTOR_KP_DEFAULT 14.0
#define MOTOR_KP_MIN 0.0
#define MOTOR_KP_MAX 50.0
#define MOTOR_KI_DEFAULT 0.01
#define MOTOR_KI_MIN 0.0
#define MOTOR_KI_MAX 1.0
#define MOTOR_KD_DEFAULT 550.0
#define MOTOR_KD_MIN 0.0
#define MOTOR_KD_MAX 2000.0

#define BALANCE_KP_DEFAULT 0.0
#define BALANCE_KP_MIN 0.0
#define BALANCE_KP_MAX 1.0
#define BALANCE_KI_DEFAULT 0.5
#define BALANCE_KI_MIN 0.0
#define BALANCE_KI_MAX 0.05
#define BALANCE_KD_DEFAULT 100.0
#define BALANCE_KD_MIN 0.0
#define BALANCE_KD_MAX 20

#define CHANNEL_DEFAULT 76U

#define MAX_SPEED_DEFAULT 0.05
#define MAX_SPEED_MAX 0.30
#define MAX_SPEED_MIN 0.0

#define MAX_TURN_DEFAULT 0.50
#define MAX_TURN_MAX 2.55
#define MAX_TURN_MIN 0.00

#define MIN_MOTOR_SPEED_DEFAULT 20
#define MIN_MOTOR_SPEED_MIN 0
#define MIN_MOTOR_SPEED_MAX 255

#endif