#include "rttserial.h"

#if defined(SERIAL_OUTPUT)
void sendString(const char *str)
{
    SEGGER_RTT_WriteString(0, str);
}

void sendInt(const char *str, int value)
{
    SEGGER_RTT_printf(0, str, value);
}
#endif