#include "Arduino.h"
#include "Motor.h"
#include "RobotController.h"
#include "RobotStorage.h"

Motor::Motor(uint8_t dir1Pin, uint8_t dir2Pin, uint8_t speedPin)
{
	_dir1Pin = dir1Pin;
	_dir2Pin = dir2Pin;
	_speedPin = speedPin;

	pinMode(dir1Pin, OUTPUT);
	pinMode(dir2Pin, OUTPUT);
	pinMode(speedPin, OUTPUT);
}

void Motor::forward(uint8_t speed)
{
	digitalWrite(_dir1Pin, LOW);
	digitalWrite(_dir2Pin, HIGH);
	analogWrite(_speedPin, speed);
}

void Motor::reverse(uint8_t speed)
{
	digitalWrite(_dir1Pin, HIGH);
	digitalWrite(_dir2Pin, LOW);
	analogWrite(_speedPin, speed);
}

void Motor::setVelocity(int16_t velocity)
{
	bool isReverse = false;
	if(velocity < 0)
	{
		velocity = -velocity;
		isReverse = true;
	}

	if (velocity > 0)
	{
		velocity += RobotStorage.data.values.minSpeed;
		if (velocity > MAX_MOTOR_SPEED)
		{
			velocity = MAX_MOTOR_SPEED;
		}
		if (isReverse)
		{
			reverse((uint8_t)velocity);
		}
		else
		{
			forward((uint8_t)velocity);
		}
	}
	else
	{
		stop();
	}
}

void Motor::stop()
{
	analogWrite(_speedPin, 0);
}