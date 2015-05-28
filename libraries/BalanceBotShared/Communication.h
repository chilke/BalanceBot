#ifndef Communication_h
#define Communication_h

#define MAX_PAYLOAD_SIZE 32

enum CommunicationCommand: uint16_t
{
    CCNack = 0x0000,
    CCControls = 0x0003,
    CCSetReadingPipe = 0x0030,
    CCSetWritingPipe = 0x0300,
    CCUpdateMotorKp = 0x3000,
    CCUpdateMotorKi = 0x000C,
    CCUpdateMotorKd = 0x00C0,
    CCUpdateBalanceKp = 0x0C00,
    CCUpdateBalanceKi = 0xC000,
    CCUpdateBalanceKd = 0x0033,
    CCSetChannel = 0x0330,
    CCGetBattery = 0x3300,
    CCUpdateMinSpeed = 0x3003,
    CCUpdateMaxSpeed = 0x0303,
    CCCommit = 0x3030,
    CCRestore = 0x00CC,
    CCResetDefaults = 0x0CC0,
    CCUpdateMaxTurn = 0xCC00,
};

struct CommunicationPayload
{
    CommunicationCommand command;
    uint8_t data[MAX_PAYLOAD_SIZE-sizeof(CommunicationCommand)];
};

#endif