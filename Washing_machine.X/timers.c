#include <xc.h>
#include "timers.h"

void init_timer2(void)
{
    //timer2 control register
    
    PR2 = 250;
    // enable timer2 interrupt
    TMR2IE = 1;
   
    TMR2ON = 0;
    
    
}
