#ifndef RemoteStorage_h
#define RemoteStorage_h

#include "Arduino.h"
#include "ControlState.h"
#include "Joystick.h"

struct RS_VALUES
{
    uint64_t readingPipe;
    uint64_t writingPipe;
    int16_t idleTimeout;
    int16_t powerTimeout;
    uint8_t channel;
    ControlStateFormat controlFormat;
    JoystickCalibrationData leftCalibration;
    JoystickCalibrationData rightCalibration;
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
    void resetDefaults();

    RS_DATA data;
private:
    int16_t commitPosition;
};

extern RS RemoteStorage;

#endif