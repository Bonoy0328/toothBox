#include "GloVarDef.h"
#include "gpio.h"

void Led_Port_Init(void);
void Motor_Port_Init(void);

#define OpenLed1    Gpio_SetIO(3,4,TRUE);
#define CloseLed1   Gpio_SetIO(3,4,FALSE);
#define OpenLed2    Gpio_SetIO(0,2,TRUE);
#define CloseLed2   Gpio_SetIO(0,2,FALSE);
#define OpenLed3    Gpio_SetIO(0,3,TRUE);
#define CloseLed3   Gpio_SetIO(0,3,FALSE);
#define OpenLed4    Gpio_SetIO(0,1,TRUE);
#define CloseLed4   Gpio_SetIO(0,1,FALSE);


#define MotorRotate Gpio_SetIO(3,2,FALSE);\
                    Gpio_SetIO(2,5,FALSE);\
                    Gpio_SetIO(3,3,TRUE);\
                    Gpio_SetIO(2,3,TRUE);

#define MotorRotateRev Gpio_SetIO(3,2,TRUE);\
                    Gpio_SetIO(2,5,TRUE);\
                    Gpio_SetIO(3,3,FALSE);\
                    Gpio_SetIO(2,3,FALSE);

int32_t main(void)
{
    Clk_SwitchTo(ClkRCL);
    Clk_SetRCHFreq(ClkFreq24Mhz);
    Clk_SwitchTo(ClkRCH);
    
    Led_Port_Init();
    Motor_Port_Init();

    while(1)
    {
        CloseLed1;
        CloseLed2;
        CloseLed3;
        CloseLed4;
        // MotorRotate;
        delay1ms(500);
        OpenLed1;
        OpenLed2;
        OpenLed3;
        OpenLed4;
        // MotorRotateRev;
        delay1ms(500);       
    }
}

/************LED Port Init****************/ 
void Led_Port_Init(void)
{
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO 外设时钟使能
    Gpio_InitIOExt(0, 1, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
    Gpio_InitIOExt(0, 2, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
    Gpio_InitIOExt(0, 3, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
    Gpio_InitIOExt(3, 4, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
}

/*********************************************/
/************Motor Port Init****************/ 
void Motor_Port_Init(void)
{
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO 外设时钟使能
    Gpio_SetIO(3,2,FALSE);
    Gpio_SetIO(2,3,FALSE);
    Gpio_SetIO(2,5,TRUE);
    Gpio_SetIO(3,3,TRUE);
    Gpio_InitIOExt(3, 2, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
    Gpio_InitIOExt(2, 3, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
    Gpio_InitIOExt(3, 3, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
    Gpio_InitIOExt(2, 5, GpioDirOut,TRUE,TRUE,FALSE,FALSE);
}










