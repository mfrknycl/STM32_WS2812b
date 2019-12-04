#include "ws2811.h"
#include "main.h"


void Delay_us (int us)
{
#if 0
  // it would be nice if chibios supported (busywaiting) delays
  // with microsecond resolution.
  //
  osalSysPolledDelayX (US2RTT(us));
#else
  for ( us *= 48/6;us>0;us--)
    __asm__("  nop;");
#endif
}



void delayUS(uint32_t us) {
	volatile uint32_t counter = (1.06)*us;
	while(counter--);
}


/*
void Delay(int counter)
{
    while(counter--);
}
*/


