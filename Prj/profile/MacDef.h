#ifndef __MACDEF_H__
#define __MACDEF_H__

#define DEBUG

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

#define MainTaskID      0x0001

#endif // !__MACDEF_H__




