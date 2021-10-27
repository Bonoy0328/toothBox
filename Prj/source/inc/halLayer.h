#ifndef __HALLAYER_H__
#define __HALLAYER_H__
#include "GloVarReDef.h"

#include "gpio.h"
#include "bt.h"
#include "lpuart.h"
#include "stdio.h"
#include "adc.h"

void Led_Port_Init(void);
void Motor_Port_Init(void);
void AdcInit(void);
void DebugInit(void);
uint16_t GetAdcValue(uint8_t flag);
void Bt1Init(void);
void HAL_SystemInit(void);
#endif // !__HALLAYER_H__


