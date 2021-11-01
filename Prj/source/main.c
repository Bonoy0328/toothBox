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
    HAL_SystemInit();
	StartTaskScheduler();
	return 1;												
}


