#ifndef __HALLAYER_H__
#define __HALLAYER_H__
#include "GloVarReDef.h"

#include "gpio.h"
#include "bt.h"
#include "lpuart.h"
#include "stdio.h"
#include "adc.h"

#define OpenLed1    Gpio_SetIO(3,4,TRUE);
#define CloseLed1   Gpio_SetIO(3,4,FALSE);
#define OpenLed2    Gpio_SetIO(0,2,TRUE);
#define CloseLed2   Gpio_SetIO(0,2,FALSE);
#define OpenLed3    Gpio_SetIO(0,3,TRUE);
#define CloseLed3   Gpio_SetIO(0,3,FALSE);
#define OpenLed4    Gpio_SetIO(0,1,TRUE);
#define CloseLed4   Gpio_SetIO(0,1,FALSE);

#define MotorRotate Gpio_SetIO(2,3,FALSE);\
                    Gpio_SetIO(2,5,TRUE);\
                    delay1ms(300);\
                    Gpio_SetIO(3,2,TRUE);\
                    Gpio_SetIO(3,3,FALSE);

#define MotorRotateRev  Gpio_SetIO(3,2,FALSE);\
                        Gpio_SetIO(3,3,TRUE);\
                        delay1ms(300);\
                        Gpio_SetIO(2,3,TRUE);\
                        Gpio_SetIO(2,5,FALSE);

#define MotorStop   Gpio_SetIO(3,2,FALSE);\
                    Gpio_SetIO(3,3,TRUE);\
                    delay1ms(300);\
                    Gpio_SetIO(2,3,FALSE);\
                    Gpio_SetIO(2,5,TRUE);\

void Led_Port_Init(void);
void Motor_Port_Init(void);
void AdcInit(void);
void DebugInit(void);
uint16_t GetAdcValue(uint8_t flag);
void Bt1Init(void);
void HAL_SystemInit(void);
#endif // !__HALLAYER_H__


