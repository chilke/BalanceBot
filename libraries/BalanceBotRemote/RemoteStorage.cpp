#include "Remote.h"

void RS::begin(bool reset)
{
    commitPosition = -1;
    if (reset)
    {
        resetDefaults();
        return;
    }

    restore();

    if (data.values.readingPipe == 0xffffffffffffffffLL)
    {
        resetDefaults();
        commit();
        return;
    }
}

void RS::resetDefaults()
{
	data.values.readingPipe = REMOTE_READING_PIPE_DEFAULT;
	data.values.writingPipe = REMOTE_WRITING_PIPE_DEFAULT;
	data.values.channel = CHANNEL_DEFAULT;

    data.values.idleTimeout = REMOTE_IDLE_TIMEOUT_DEFAULT;
    data.values.powerTimeout = REMOTE_POWER_TIMEOUT_DEFAULT;

    data.values.controlFormat = (ControlStateFormat)0;

    data.values.leftCalibration.zeroX = 512;
    data.values.leftCalibration.minX = 2;
    data.values.leftCalibration.maxX = 1020;
    data.values.leftCalibration.zeroY = 497;
    data.values.leftCalibration.minY = 2;
    data.values.leftCalibration.maxY = 1020;
    data.values.leftCalibration.deadZone = 10;

    data.values.rightCalibration.zeroX = 508;
    data.values.rightCalibration.minX = 2;
    data.values.rightCalibration.maxX = 1020;
    data.values.rightCalibration.zeroY = 506;
    data.values.rightCalibration.minY = 2;
    data.values.rightCalibration.maxY = 1020;
    data.values.rightCalibration.deadZone = 10;
}

void RS::restore()
{
    uint8_t index;

    for (index = 0; index < sizeof(data); index++)
    {
        data.data[index] = eeprom_read_byte((uint8_t *)index);
    }
}

void RS::doWork()
{
    if (commitPosition >= 0 && eeprom_is_ready())
    {
        eeprom_update_byte((uint8_t *)commitPosition,
            data.data[commitPosition]);
        ++commitPosition;
        if (commitPosition >= sizeof(RS_VALUES))
        {
            commitPosition = -1;
        }
    }
}

void RS::commit()
{
    commitPosition = 0;
}

void RS::debug()
{
    Serial.print("ControlFormat="); Serial.println(data.values.controlFormat);
    data.values.leftCalibration.debug();
    data.values.rightCalibration.debug();
}

RS RemoteStorage;