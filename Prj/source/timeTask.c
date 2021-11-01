#include "timeTask.h"

/************************************************************
 * Personal task declaration
 * **********************************************************/
void MainTask(void);

/***************************************************
 * trigger a task, execute the task immediately
 * TaskID : the Task ID, one hot code
 *          Example : 0x0001 or 0x0002
 * callback : task callback function
 *          Example : void MainTask(void)
 * **************************************************/
void SetTaskEvent(uint16_t TaskID,void (* callBack)(void)){
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
void StartTask(uint16_t TaskID,uint16_t time,void (* callBack)(void)){
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
void StopTask(uint16_t TaskID){
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

/************************************************************
 * start Task Scheduler
 * **********************************************************/
void StartTaskScheduler(void)
{
    // Clear Task list
    uint8_t i;
    for(i = 0;i < 16;i++)
    {
        TaskList[i].TaskID = 0;
        TaskList[i].time = 0;
        TaskList[i].timePoint = 0;
    }
    // start Task
    StartTask(MainTaskID,1000,MainTask);

    // start Task Scheduler
    while(1)TaskScheduler();
    
}
/************************************************************
 * Task Scheduler
 * **********************************************************/
void TaskScheduler(void)
{
    uint8_t i;
    for(i = 0;i < 16;i++){
        if(TaskList[i].TaskID){
            if((systemVar.timeline - TaskList[i].timePoint)>=TaskList[i].time)
            {
                TaskList[i].TaskID = 0;
                TaskList[i].callBack();
            }
        }
    }
}

/************************************************************
 * Personal task
 * **********************************************************/
void MainTask(void)
{
    printf("test for scheduler\r\n");
    StartTask(MainTaskID,1000,MainTask);
}






