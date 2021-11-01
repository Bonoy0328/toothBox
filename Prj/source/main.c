#include "GloVarDef.h"
#include "timeTask.h"

#ifdef DEBUG
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{
    LPUart_SendData(ch);
    return ch;
}
#endif // DEBUG

int32_t main(void)
{
    // Hardware initialization
    HAL_SystemInit();
    // Software initialization
	StartTaskScheduler();
	return 1;												
}


