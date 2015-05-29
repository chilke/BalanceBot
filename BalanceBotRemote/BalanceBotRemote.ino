#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "LiquidCrystal_I2C.h"
#include "Remote.h"
#include "BotDefines.h"
#include "RF24.h"
#include "SPI.h"


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  RemoteController.begin();
}

void loop() {
  RemoteController.doWork();
}
