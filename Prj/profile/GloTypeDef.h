#ifndef __GLOTYPEDEF_H__
#define __GLOTYPEDEF_H__

#include "ddl.h"
#include "MacDef.h"

typedef struct
{
    uint16_t PowerAdcValue;
    uint16_t MotorAdcValue;
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

typedef union
{
    struct
    {
        uint8_t start1Ms:1;
    }bit;
    uint8_t TaskFlag;
}Task_t;



#endif // !__GLOTYPEDEF_H__

