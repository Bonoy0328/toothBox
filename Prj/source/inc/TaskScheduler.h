#ifndef __TASKSCHEDULER_H__
#define __TASKSCHEDULER_H__
#include "halLayer.h"
void SetTaskEvent(uint16_t TaskID,void (* callBack)(void));
void StartTask(uint16_t TaskID,uint16_t time,void (* callBack)(void));
void StopTask(uint16_t TaskID);
void StartTaskScheduler(void);

#endif // !__TASKSCHEDULER_H__








