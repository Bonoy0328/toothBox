#include "GloVarDef.h"
#include "timeTask.h"
#include "FreeRTOS.h"
#include "task.h"

#ifdef DEBUG
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
    LPUart_SendData(ch);
    return ch;
}
#endif // DEBUG

void testTask(void * pvParameters);
TaskHandle_t testTask_Handler;

int32_t main(void)
{
    HAL_SystemInit();
	SysTick_Config(SystemCoreClock);
		printf("test for freeRTOS\r\n");
	xTaskCreate((TaskFunction_t)         testTask,
				(char * )                TEST_TASK_NAME,
				(configSTACK_DEPTH_TYPE) TEST_TASK_STK_SIZE,
				(void *)                 NULL,
				(UBaseType_t)            TEST_TASK_PRIO,
				(TaskHandle_t *)         &testTask_Handler );
	vTaskStartScheduler();
				while(1){
				}
	//return 1;												
}

void testTask(void * pvParameters)
{
	while (1)
	{
		printf("test for freeRTOS\r\n");
		vTaskDelay(500);
	}
	
}


