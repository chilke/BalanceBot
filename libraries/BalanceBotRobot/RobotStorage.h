#ifndef RobotStorage_h
#define RobotStorage_h

#include "Arduino.h"
#include "BotDefines.h"

struct RS_VALUES
{
    uint64_t readingPipe;
    uint64_t writingPipe;
    float kp;
    float ki;
    float kd;
    float trim;
    float minSpeed;
    float maxSpeed;
    float maxTurn;

    uint8_t leanCount;
    uint8_t channel;
};

union RS_DATA
{
    RS_VALUES values;
    uint8_t data[sizeof(RS_VALUES)];
};

class RS
{
public:
    void begin(bool reset);
    void doWork();
    void commit();
    void restore();
    void debug();
    void resetDefaults();

    RS_DATA data;
private:
    int16_t commitPosition;
};

extern RS RobotStorage;

#endif