#ifndef RobotCommunication_h
#define RobotCommunication_h

#include "Arduino.h"
#include "RF24.h"
#include "BotDefines.h"
#include "Communication.h"

class RobotCommunication 
{
public:
    RobotCommunication(uint8_t cePin, uint8_t csnPin);
    void begin();
    void sendCommand(CommunicationCommand command, uint8_t *data,
        uint8_t length);
    void checkReceive();
private:
    bool isSetup;
    RF24 radio;
    CommunicationPayload receivePayload;
    uint8_t receiveLength;
    CommunicationPayload sendPayload;
};

#endif