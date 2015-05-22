#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor
{
public:
	Motor(uint8_t dir1Pin, uint8_t dir2Pin, uint8_t speedPin);
	void forward(uint8_t speed);
	void reverse(uint8_t speed);
	void setVelocity(int16_t velocity);
	void stop();
private:
	uint8_t _dir1Pin;
	uint8_t _dir2Pin;
	uint8_t _speedPin;
};

#endif