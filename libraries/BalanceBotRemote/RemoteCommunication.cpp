#include "RemoteCommunication.h"
#include "RemoteController.h"
#include "RemoteStorage.h"

RemoteCommunication::RemoteCommunication(uint8_t cePin, uint8_t csnPin):
radio(cePin, csnPin)
{
    isSetup = false;
}

void RemoteCommunication::begin()
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

    radio.openWritingPipe(RemoteStorage.data.values.writingPipe);
    radio.openReadingPipe(1, RemoteStorage.data.values.readingPipe);
    radio.enableDynamicPayloads();
    radio.setAutoAck(true);
    radio.powerUp();
    radio.startListening();

    isConnected = false;
    isWaitingResponse = false;
}

void RemoteCommunication::checkReceive()
{
    if (radio.available())
    {
        bool done = false;
        while (!done)
        {
            receiveLength = radio.getDynamicPayloadSize();
            done = radio.read((void *)&receivePayload, receiveLength);
        }

        RemoteController.handleCommCommand(receivePayload.command,
            receivePayload.data, receiveLength-sizeof(CommunicationCommand));
    }
}

void RemoteCommunication::sendCommand(CommunicationCommand command, uint8_t *data,
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

    isWaitingResponse = true;
    radio.startListening();
}