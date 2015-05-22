#ifndef Communication_h
#define Communication_h

#define MAX_PAYLOAD_SIZE 32

enum CommunicationCommand: uint16_t
{
    CCNack = 0x0000,
    CCControls = 0x0003,
    CCSetReadingPipe = 0x0030,
    CCSetWritingPipe = 0x0300,
    CCUpdateKp = 0x3000,
    CCUpdateKi = 0x000C,
    CCUpdateKd = 0x00C0,
    CCUpdateTrim = 0x0C00,
    CCSetChannel = 0xC000,
    CCGetBattery = 0x0033,
    CCUpdateMinSpeed = 0x0330,
    CCUpdateMaxSpeed = 0x3300,
    CCUpdateLeanCount = 0x3003,
    CCCommit = 0x00CC,
    CCRestore = 0x0CC0,
    CCResetDefaults = 0xCC00,
    CCUpdateMaxTurn = 0xC00C,
};

struct CommunicationPayload
{
    CommunicationCommand command;
    uint8_t data[MAX_PAYLOAD_SIZE-sizeof(CommunicationCommand)];
};

#endif