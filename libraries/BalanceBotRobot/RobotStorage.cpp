#include "Robot.h"
#include "BotDefines.h"
#include "avr/eeprom.h"

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
    data.values.readingPipe = ROBOT_READING_PIPE_DEFAULT;
    data.values.writingPipe = ROBOT_WRITING_PIPE_DEFAULT;
    data.values.motorKp = MOTOR_KP_DEFAULT;
    data.values.motorKi = MOTOR_KI_DEFAULT;
    data.values.motorKd = MOTOR_KD_DEFAULT;
    data.values.balanceKp = BALANCE_KP_DEFAULT;
    data.values.balanceKi = BALANCE_KI_DEFAULT;
    data.values.balanceKd = BALANCE_KD_DEFAULT;
    data.values.minSpeed = MIN_MOTOR_SPEED_DEFAULT;
    data.values.maxSpeed = MAX_SPEED_DEFAULT;
    data.values.maxTurn = MAX_TURN_DEFAULT;
    data.values.channel = CHANNEL_DEFAULT;
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

void print64(uint64_t value)
{
    int8_t i;
    uint8_t j;
    for (i = 7; i >= 0; i--)
    {
        j = (uint8_t)(value >> (i*8));
        Serial.print(j, HEX);
    }
}

RS RobotStorage;