#include "timeTask.h"
#include "TaskScheduler.h"
/************************************************************
 * Personal task declaration
 * **********************************************************/
void MainTask(void)
{
    static uint16_t ItrValueFilter[ITRMAXLEN] = {0};
    static uint8_t index = 0;
    uint8_t i;
    if(!systemFlag.motorRuning){
        ItrValueFilter[index++] = GetAdcValue(ITR);
        if(ItrValueFilter[index - 1] > (uint16_t)(TrigerScale*GetBufAvg(ItrValueFilter))){
            systemFlag.motorRuning = 1;
            SetTaskEvent(CurrentTaskID,MotorCurrentDetectTask);
            MotorRotate;
        }
        index %= ITRMAXLEN;
    }
    // printf("test for scheduler\r\n");
    StartTask(MainTaskID,100,MainTask);
}

uint16_t GetBufAvg(uint16_t *ItrValueFilter)
{
    uint16_t res = 0;
    uint8_t i;
    for(i = 0;i < ITRMAXLEN;i++)res += *(ItrValueFilter+i);
    return (uint16_t)(res / ITRMAXLEN);
}

void MotorReverFun(void)
{
    MotorRotateRev;
    systemFlag.motorReRuning = 1;
    SetTaskEvent(CurrentTaskID,MotorCurrentDetectTask);
}

void MotorCurrentDetectTask(void)
{
    static uint16_t motorCurrent;
    motorCurrent = GetAdcValue(MOTOR);
    if(motorCurrent > MotorCurrentshledValue){
        MotorStop;
        if(systemFlag.motorRuning){
            systemFlag.motorRuning = 0;
            StartTask(MotorReverTaskID,MotorStopTime,MotorReverFun);
        }else if(systemFlag.motorReRuning){
            systemFlag.motorReRuning = 0;
        }
    }
    if(systemFlag.motorRuning || systemFlag.motorReRuning){
        StartTask(CurrentTaskID,100,MotorCurrentDetectTask);
    }

    

}

/************************************************************
 * start Task Scheduler
 * **********************************************************/
void StartTaskScheduler(void)
{
    // Clear Task list
    uint8_t i;
    for(i = 0;i < 16;i++)
    {
        TaskList[i].TaskID    = 0;
        TaskList[i].time      = 0;
        TaskList[i].timePoint = 0;
    }
    // start Task
    StartTask(MainTaskID,1000,MainTask);
    // start Task Scheduler
    while(1)TaskScheduler();
    
}


