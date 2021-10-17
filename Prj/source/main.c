#include "GloVarDef.h"
#include "gpio.h"
#include "bt.h"
#include "lpuart.h"
#include "stdio.h"
#ifdef DEBUG
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
    LPUart_SendData(ch);
    return ch;
}
#endif // DEBUG


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

#define MotorRotate Gpio_SetIO(2,3,FALSE);\
                    Gpio_SetIO(2,5,TRUE);\
										delay1ms(300);\
                    Gpio_SetIO(3,2,TRUE);\
                    Gpio_SetIO(3,3,FALSE);

#define MotorRotateRev Gpio_SetIO(3,2,FALSE);\
                    Gpio_SetIO(3,3,TRUE);\
										delay1ms(300);\
										Gpio_SetIO(2,3,TRUE);\
                    Gpio_SetIO(2,5,FALSE);

int32_t main(void)
{

#ifdef DEBUG
    uint16_t u16timer;
    uint32_t u32sclk;

    stc_lpuart_config_t  stcConfig;
    stc_lpuart_irq_cb_t stcLPUartIrqCb;
    stc_lpuart_multimode_t stcMulti;
    stc_lpuart_sclk_sel_t  stcLpuart_clk;
    stc_lpuart_mode_t       stcRunMode;
    stc_lpuart_baud_config_t  stcBaud;
    stc_bt_config_t stcBtConfig;
    
    DDL_ZERO_STRUCT(stcConfig);
    DDL_ZERO_STRUCT(stcLPUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBtConfig);
#endif // DEBUG

stc_clk_config_t stcCfg;

    Clk_SwitchTo(ClkRCL);
    Clk_SetRCHFreq(ClkFreq24Mhz);
    Clk_SwitchTo(ClkRCH);
    
    stcCfg.enClkSrc = ClkRCH;
    stcCfg.enHClkDiv = ClkDiv1;
    stcCfg.enPClkDiv = ClkDiv1;
    Clk_Init(&stcCfg);

    // Led_Port_Init();
    Motor_Port_Init();

#ifdef DEBUG

    Clk_SetPeripheralGate(ClkPeripheralLpUart,TRUE);//使能LPUART时钟
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);

    Gpio_InitIOExt(3,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    // Gpio_InitIOExt(2,6,GpioDirOut,TRUE,FALSE,FALSE,FALSE);

    Gpio_SetFunc_UART2TX_P34();

    stcLpuart_clk.enSclk_sel = LPUart_Pclk; //LPUart_Rcl;
    stcLpuart_clk.enSclk_Prs = LPUartDiv1;
    stcConfig.pstcLpuart_clk = &stcLpuart_clk;

    stcRunMode.enLpMode = LPUartNoLPMode;//正常工作模式或低功耗工作模式配置
    stcRunMode.enMode   = LPUartMode1;
    stcConfig.pstcRunMode = &stcRunMode;

    stcLPUartIrqCb.pfnRxIrqCb = NULL;
    stcLPUartIrqCb.pfnTxIrqCb = NULL;
    stcLPUartIrqCb.pfnRxErrIrqCb = NULL;
    stcConfig.pstcIrqCb = &stcLPUartIrqCb;
    stcConfig.bTouchNvic = FALSE;
    stcMulti.enMulti_mode = LPUartNormal;//只有模式2/3才有多主机模式

    stcConfig.pstcMultiMode = &stcMulti;
   
    LPUart_EnableIrq(LPUartRxIrq);

    LPUart_Init(&stcConfig);

    if(LPUart_Pclk == stcLpuart_clk.enSclk_sel)
        u32sclk = Clk_GetPClkFreq();
    else if(LPUart_Rcl == stcLpuart_clk.enSclk_sel)
        u32sclk = 38400;//此处建议用户使用内部38.4K时钟，如果用户使用32.768K时钟的，此处更新成32768
    else
        u32sclk = 32768;

    stcBaud.u32Baud = 9600;
    stcBaud.bDbaud = 0;
    stcBaud.u8LpMode = LPUartNoLPMode;
    stcBaud.u8Mode = LPUartMode1;
    u16timer = LPUart_SetBaudRate(u32sclk,stcLpuart_clk.enSclk_Prs,&stcBaud);
    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    stcBtConfig.enTog = BtTogEnable;
    Bt_Init(TIM2, &stcBtConfig);//调用basetimer2设置函数产生波特率
    Bt_ARRSet(TIM2,u16timer);
    Bt_Cnt16Set(TIM2,u16timer);
    Bt_Run(TIM2);

#endif // DEBUG

    while(1)
    {
      // MotorRotate;
      // CloseLed1;
      // CloseLed2;
      // CloseLed3;
      // CloseLed4;
      // delay1ms(1000);
      // MotorRotateRev;
      // OpenLed1;
      // OpenLed2;
      // OpenLed3;
      // OpenLed4;
      // delay1ms(1000);
      printf("test for uart\r\n");
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










