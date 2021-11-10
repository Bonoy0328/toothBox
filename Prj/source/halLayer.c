#include "halLayer.h"

void HAL_SystemInit(void)
{
    stc_clk_config_t stcCfg;

    Clk_SwitchTo(ClkRCL);
    Clk_SetRCHFreq(ClkFreq24Mhz);
    Clk_SwitchTo(ClkRCH);
    
    stcCfg.enClkSrc = ClkRCH;
    stcCfg.enHClkDiv = ClkDiv1;
    stcCfg.enPClkDiv = ClkDiv1;
    Clk_Init(&stcCfg);

    // Led_Port_Init();
    DebugInit();
    Motor_Port_Init();
    AdcInit();
}

void AdcInit(void)
{
    // ADC Param
    stc_adc_cfg_t      stcAdcCfg;
    stc_adc_norm_cfg_t stcAdcNormCfg;

    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcNormCfg);

    // ADC
    Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE);
    Gpio_SetAnalog(2, 4, TRUE);  // BAT
    Gpio_SetAnalog(2, 6, TRUE);  // ITR
    Gpio_SetAnalog(3, 5, TRUE);  // MOTRO1
    Gpio_SetAnalog(3, 6, TRUE);  // MOTOR2
    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;                // BGR必须使能
    M0P_BGR->CR_f.TS_EN  = 0x0u;
    delay100us(1);

    stcAdcCfg.enAdcOpMode = AdcNormalMode;          //单次采样模式
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv1;         //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk;   //4个采样时钟
    stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;   //参考电压:内部2.5V(avdd>3V,SPS<=200kHz)  SPS速率 = ADC时钟 / (采样时钟 + 16CLK) 
    stcAdcCfg.bAdcInBufEn = FALSE;                  //电压跟随器如果使能，SPS采样速率 <=200K
    stcAdcCfg.u32AdcRegHighThd = 0u;                //比较阈值上门限
    stcAdcCfg.u32AdcRegLowThd = 0u;                 //比较阈值下门限
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;       //ADC转换自动触发设置
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);    
    stcAdcNormCfg.enAdcNormModeCh = AdcExInputCH1;  //通道1 P26
    stcAdcNormCfg.bAdcResultAccEn = FALSE;
    Adc_ConfigNormMode(&stcAdcCfg, &stcAdcNormCfg);
}

uint16_t GetAdcValue(uint8_t flag)
{
    uint16_t res;
    if(flag == ITR){
        M0P_ADC->CR0_f.SEL = AdcExInputCH1;
    }else if(flag == MOTOR){
        if(systemFlag.MotorDir){
            M0P_ADC->CR0_f.SEL = AdcExInputCH5;
        }else{
            M0P_ADC->CR0_f.SEL = AdcExInputCH6;
        }
    }else if(flag == BAT){
        M0P_ADC->CR0_f.SEL = AdcExInputCH0;
    }
    Adc_Start();
    while(FALSE != Adc_PollBusyState());
    Adc_GetResult(&res);
    return res;
}

void DebugInit(void)
{
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
    Clk_SetPeripheralGate(ClkPeripheralLpUart,TRUE);//使能LPUART时钟
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);

    Gpio_InitIOExt(3,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    // Gpio_InitIOExt(2,6,GpioDirOut,TRUE,FALSE,FALSE,FALSE);

    Gpio_SetFunc_UART2TX_P34();

    stcLpuart_clk.enSclk_sel = LPUart_Pclk;     //LPUart_Rcl;
    stcLpuart_clk.enSclk_Prs = LPUartDiv1;
    stcConfig.pstcLpuart_clk = &stcLpuart_clk;

    stcRunMode.enLpMode = LPUartNoLPMode;       //正常工作模式或低功耗工作模式配置
    stcRunMode.enMode   = LPUartMode1;
    stcConfig.pstcRunMode = &stcRunMode;

    stcLPUartIrqCb.pfnRxIrqCb = NULL;
    stcLPUartIrqCb.pfnTxIrqCb = NULL;
    stcLPUartIrqCb.pfnRxErrIrqCb = NULL;
    stcConfig.pstcIrqCb = &stcLPUartIrqCb;
    stcConfig.bTouchNvic = FALSE;
    stcMulti.enMulti_mode = LPUartNormal;      //只有模式2/3才有多主机模式

    stcConfig.pstcMultiMode = &stcMulti;
   
    LPUart_EnableIrq(LPUartRxIrq);

    LPUart_Init(&stcConfig);

    if(LPUart_Pclk == stcLpuart_clk.enSclk_sel)
        u32sclk = Clk_GetPClkFreq();
    else if(LPUart_Rcl == stcLpuart_clk.enSclk_sel)
        u32sclk = 38400;                        //此处建议用户使用内部38.4K时钟，如果用户使用32.768K时钟的，此处更新成32768
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
    Bt_Init(TIM2, &stcBtConfig);                //调用basetimer2设置函数产生波特率
    Bt_ARRSet(TIM2,u16timer);
    Bt_Cnt16Set(TIM2,u16timer);
    Bt_Run(TIM2);

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

/***************system clock init********************/

void Bt1Int(void)
{
    if (TRUE == Bt_GetIntFlag(TIM1))
    {
        systemVar.timeline++;
        Bt_ClearIntFlag(TIM1);
    }
}

void Bt1Init(void)
{
    stc_bt_config_t   stcConfig;
    
    Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);

    stcConfig.pfnTim1Cb = Bt1Int;

    stcConfig.enGateP = BtPositive;
    stcConfig.enGate  = BtGateDisable;
    stcConfig.enPRS   = BtPCLKDiv64;
    stcConfig.enTog   = BtTogDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enMD    = BtMode2;
    //Bt初始化
    if (Ok != Bt_Init(TIM1, &stcConfig))
    {
        return;
    }
    
    //TIM1中断使能
    Bt_ClearIntFlag(TIM1);
    Bt_EnableIrq(TIM1);
    EnableNvic(TIM1_IRQn, 3, TRUE);
    
    //设置重载值和计数值，启动计数
    // 1ms 定时
    Bt_ARRSet(TIM1, 0xFE89);
    Bt_Cnt16Set(TIM1, 0xFE89);
    Bt_Run(TIM1);
}






