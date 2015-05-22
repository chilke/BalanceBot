#ifndef ControlState_h
#define ControlState_h

enum ControlStateFormat : uint8_t
{
    FormatOne=0,
    FormatOneStickLeft=0,
    FormatOneStickRight=1,
    FormatTwo=2,
    FormatTwoStick=2,
    FormatRCLeft=3,
    FormatRCRight=4,
    FormatEnd=5
};

struct ControlState1
{
    ControlStateFormat format;
    int8_t speed;
    int8_t turn;
};

struct ControlState2
{
    ControlStateFormat format;
    int8_t left;
    int8_t right;
};

union ControlState
{
    ControlStateFormat format;
    ControlState1 format1;
    ControlState2 format2;
};

#endif