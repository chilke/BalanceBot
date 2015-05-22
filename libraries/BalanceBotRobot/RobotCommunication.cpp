#include "RobotCommunication.h"
#include "RobotController.h"
#include "RobotStorage.h"

RobotCommunication::RobotCommunication(uint8_t cePin, uint8_t csnPin):
    radio(cePin, csnPin)
{
    isSetup = false;
}

void RobotCommunication::begin()
{
    if (!isSetup)
    {
        radio.begin();
        isSetup = true;
    }
    else
    {
        radio.stopListening();
    }

    radio.openWritingPipe(RobotStorage.data.values.writingPipe);
    radio.openReadingPipe(1, RobotStorage.data.values.readingPipe);
    radio.enableDynamicPayloads();
    radio.setAutoAck(true);
    radio.powerUp();
    radio.startListening();
}

void RobotCommunication::checkReceive()
{
    if (radio.available())
    {
        bool done = false;
        while (!done)
        {
            receiveLength = radio.getDynamicPayloadSize();
            done = radio.read((void *)&receivePayload, receiveLength);
        }

        RobotController.handleCommCommand(receivePayload.command,
            receivePayload.data, receiveLength-sizeof(CommunicationCommand));
    }
}

void RobotCommunication::sendCommand(CommunicationCommand command, uint8_t *data,
    uint8_t length)
{
    radio.stopListening();
    sendPayload.command = command;
    if (length > 0)
    {
        memcpy(sendPayload.data, data, length);
    }
    length+=sizeof(CommunicationCommand);
    radio.write((void *)&sendPayload, length);
    radio.startListening();
}