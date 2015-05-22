#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "Robot.h"
#include "BotDefines.h"
#include "RF24.h"
#include "SPI.h"

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  RobotController.begin();
  
  RobotStorage.debug();
}

void loop() {
  RobotController.doWork();
}
