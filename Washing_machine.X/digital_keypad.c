#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void) {
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode) { // <--- FIXED THIS LINE
    static unsigned char once = 1;
    static unsigned char long_pressed = 1;
    static unsigned char pre_key;

    unsigned char key = KEYPAD_PORT & INPUT_LINES;
    if ((key != ALL_RELEASED) && once) {
        once = 0;
        long_pressed = 0;
        pre_key = key;
        
    } else if ((key == ALL_RELEASED) && !once) {
        once = 1;
        if (long_pressed < 15) {
            return pre_key; // short press
        }
    } else if (!once && long_pressed <= 15) {
        long_pressed++;
    }
    else if (!once  && long_pressed == 16 && key == SW4) {
        long_pressed++;
        return LPSW4; // long press on SW4
    }

    return ALL_RELEASED;
}
