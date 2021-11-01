#ifndef __GLOTYPEDEF_H__
#define __GLOTYPEDEF_H__

#include "ddl.h"
#include "MacDef.h"

typedef struct
{
    uint32_t timeline;
    uint16_t PowerAdcValue;
    uint16_t MotorAdcValue;
    uint16_t TaskFlag;
    uint16_t TaskPreFlag;
}SystemVar_t;

typedef struct
{
    uint8_t MotorDir:1;
    uint8_t time1Ms:1;
}SystemFlag_t;

enum ADCFALG{
    MOTOR = 0,
    BAT = 1,
    ITR = 2,
};

typedef struct
{
    uint16_t TaskID;
    uint16_t time;
    uint32_t timePoint;
    void (* callBack)(void);
}Task_t;


#endif // !__GLOTYPEDEF_H__

