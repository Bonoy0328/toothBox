#include "TaskScheduler.h"
/***************************************************
 * trigger a task, execute the task immediately
 * TaskID : the Task ID, one hot code
 *          Example : 0x0001 or 0x0002
 * callback : task callback function
 *          Example : void MainTask(void)
 * **************************************************/
void SetTaskEvent(uint16_t TaskID,void (* callBack)(void))
{
    uint8_t i;
    for(i = 0;i < 16;i++)
    {
        if((TaskID >> i)&0x01){
            TaskList[i].TaskID = TaskID;
            TaskList[i].time = systemVar.timeline;
            TaskList[i].timePoint = systemVar.timeline;
            TaskList[i].callBack = callBack;
            break;
        }
    }
}

/************************************************************
 * start a Task , execute the task after a delay of $time ms
 * TaskID : the Task ID, one hot code
 *          Example : 0x0001 or 0x0002
 * time   : the delay time , MS
 * callback : task callback function
 *          Example : void MainTask(void)
 * **********************************************************/
void StartTask(uint16_t TaskID,uint16_t time,void (* callBack)(void))
{
    uint8_t i;
    for(i = 0;i < 16;i++)
    {
        if((TaskID >> i)&0x01){
            TaskList[i].TaskID = TaskID;
            TaskList[i].time = time;
            TaskList[i].timePoint = systemVar.timeline;
            TaskList[i].callBack = callBack;
            break;
        }
    }
}

/************************************************************
 * stop a Task , kill the task
 * **********************************************************/
void StopTask(uint16_t TaskID)
{
    uint8_t i;
    for(i = 0;i < 16;i++)
    {
        if((TaskID >> i)&0x01){
            TaskList[i].TaskID = 0;
            TaskList[i].time = 0;
            TaskList[i].timePoint = 0;
            break;
        }
    }
}

void GoToSleep(void)
{
    
    // 根据任务时长设置中断唤醒时间
}

/************************************************************
 * Task Scheduler
 * **********************************************************/
void TaskScheduler(void)
{
    uint8_t i;
    if(systemVar.TaskFlag){
        // if task is runing
        for(i = 0;i < 16;i++){
            if(TaskList[i].TaskID){
                if((systemVar.timeline - TaskList[i].timePoint)>=TaskList[i].time)
                {
                    TaskList[i].TaskID = 0;
                    TaskList[i].callBack();
                }
            }
        }
    }else{
        // if task not run got to sleep
        GoToSleep();
    }
}

