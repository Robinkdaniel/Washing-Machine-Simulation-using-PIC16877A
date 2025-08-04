#include "main.h"

extern unsigned char reset_flag;

char *washing_program[] = {
    "Daily", "Heavy", "Dedicates", "Whites", "Stain Wash", "Eco cottons",
    "woolens", "Bed Sheets", "Rinse+Dry", "Dry Only", "Wash Only", "Aqua Store"
};

char *water_level[] = {"Auto", "Low", "Medium", "High", "Max"};

unsigned char program_no = 0;
unsigned char level = 0;
unsigned char sec = 0, min = 0;
unsigned int total_time, wash_time, rinse_time, spin_time, time;
extern unsigned char operational_mode;

void clear_screen(void) {
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

void power_on_screen(void) {
    for (unsigned char i = 0; i < 16; i++) {
        clcd_putch(BLOCK, LINE1(i));
        __delay_ms(100);
    }
    clcd_print("Powering on", LINE2(1));
    clcd_print("Washing Machine", LINE3(1));

    for (unsigned char i = 0; i < 16; i++) {
        clcd_putch(BLOCK, LINE4(i));
        __delay_ms(100);
    }
    __delay_ms(2000);
}

void washing_program_display(unsigned char key) {
    if (reset_flag == RESET_WASH_PROGRAM_SCREEN) {
        clear_screen();
        reset_flag = RESET_NOTHING;
    }

    if (key == SW4) {
        clear_screen();
        program_no++;
        if (program_no == 12) {
            program_no = 0;
        }
        reset_flag = RESET_WASH_PROGRAM_SCREEN;
    }

    clcd_print("Washing Program", LINE1(0));
    clcd_putch('*', LINE2(0));
    if (program_no <= 9) {
        clcd_print(washing_program[program_no], LINE2(1));
        clcd_print(washing_program[(program_no + 1) % 12], LINE3(1));
        clcd_print(washing_program[(program_no + 2) % 12], LINE4(1));
    } else if (program_no == 10) {
        clcd_print(washing_program[10], LINE2(1));
        clcd_print(washing_program[11], LINE3(1));
        clcd_print(washing_program[0], LINE4(1));
    } else if (program_no == 11) {
        clcd_print(washing_program[11], LINE2(1));
        clcd_print(washing_program[0], LINE3(1));
        clcd_print(washing_program[1], LINE4(1));
    }
}

void water_level_screen(unsigned char key) {
    if (reset_flag == RESET_WATER_LEVEL_SCREEN) {
        clear_screen();
        reset_flag = RESET_NOTHING;
    }

    if (key == SW4) {
        clear_screen();
        level++;
        if (level == 5) {
            level = 0;
        }
    }

    clcd_print("WATER LEVEL:", LINE1(0));
    clcd_putch('*', LINE2(0));
    clcd_print(water_level[level], LINE2(2));
    clcd_print(water_level[(level + 1) % 5], LINE3(2));
    clcd_print(water_level[(level + 2) % 5], LINE4(2));
}

void set_time(void) {
    min = 0;
    sec = 0;

    switch (program_no) {
        case 0:
            switch (level) {
                case 1: sec = 33; break;
                case 0:
                case 2: sec = 41; break;
                case 3:
                case 4: sec = 45; break;
            }
            break;
        case 1:
            switch (level) {
                case 0:
                case 1:
                case 2: sec = 26; break;
                case 3:
                case 4: sec = 31; break;
            }
            break;
        case 2:
            switch (level) {
                case 1: sec = 43; break;
                case 0:
                case 2: sec = 50; break;
                case 3:
                case 4: sec = 57; break;
            }
            break;
        case 3: min = 1; sec = 16; break;
        case 4: min = 1; sec = 36; break;
        case 5:
            switch (level) {
                case 0:
                case 1:
                case 2: sec = 31; break;
                case 3:
                case 4: sec = 36; break;
            }
            break;
        case 7:
            switch (level) {
                case 1: sec = 46; break;
                case 0:
                case 2: sec = 53; break;
                case 3:
                case 4: sec = 57; break;
            }
            break;
        case 8:
            switch (level) {
                case 1: sec = 18; break;
                default: sec = 20; break;
            }
            break;
        case 9: sec = 6; break;
        case 10:
            switch (level) {
                case 1: sec = 16; break;
                case 0:
                case 2: sec = 21; break;
                case 3:
                case 4: sec = 26; break;
            }
            break;
        case 11: sec = 50; break;
    }
}

void run_program(unsigned char key) {
    door_status_check();
    if (reset_flag == RESET_START_SCREEN) {
        clear_screen();

        clcd_print("Prog:", LINE1(0));
        clcd_print(washing_program[program_no], LINE1(6));

        clcd_print("Time:", LINE2(0));
        clcd_putch(min / 10 + '0', LINE2(6));
        clcd_putch(min % 10 + '0', LINE2(7));
        clcd_putch(':', LINE2(8));
        clcd_putch(sec / 10 + '0', LINE2(9));
        clcd_putch(sec % 10 + '0', LINE2(10));

        reset_flag = RESET_NOTHING;
        __delay_ms(2000);
        clear_screen();
        clcd_print("Function:", LINE1(0));
        clcd_print("Time", LINE2(0));
        clcd_print("5:START 6:PAUSE", LINE3(0));

        total_time = time = min * 60 + sec;
        wash_time = (int) total_time * 0.42;
        rinse_time = (int) total_time * 0.12;
        spin_time = (int) total_time * 0.42;

        TMR2ON = 1;
        FAN = ON;
    }
    if (key == SW6) {
        TMR2ON = 0;
        FAN = OFF;
        operational_mode = PAUSE;
    }

    total_time = min * 60 + sec;
    if (program_no <= 7) {
        if (total_time >= (time - wash_time)) {
            clcd_print("wash", LINE1(10));
        } else if (total_time >= (time - wash_time - spin_time)) {
            clcd_print("Rinse", LINE1(10));
        } else {
            clcd_print("Spin ", LINE1(10));
        }
    } else if (program_no == 8) {
        if (total_time >= (time - (0.40 * time))) {
            clcd_print("Rinse", LINE1(10));
        } else {
            clcd_print("Spin ", LINE1(10));
        }
    } else if (program_no == 9) {
        clcd_print("Spin ", LINE1(10));
    } else {
        clcd_print("Wash", LINE1(0));
    }

    clcd_putch(min / 10 + '0', LINE2(6));
    clcd_putch(min % 10 + '0', LINE2(7));
    clcd_putch(':', LINE2(8));
    clcd_putch(sec / 10 + '0', LINE2(9));
    clcd_putch(sec % 10 + '0', LINE2(10));

    if (min == 0 && sec == 0) {
        clear_screen();
        FAN = OFF;
        BUZZER = ON;
        clcd_print("Program completed", LINE1(0));
        clcd_print("Remove clothes", LINE2(0));
        __delay_ms(2000);
        BUZZER = OFF;
        operational_mode = WASHING_PROGRAM_SCREEN;
        reset_flag = RESET_WASH_PROGRAM_SCREEN;
        clear_screen();
    }
}

void door_status_check()
{
    if (RB0 == 0) { // Check if RB0 button is pressed (active low)
        __delay_ms(50); // Debounce delay
        if (RB0 == 0) { // Confirm press after debounce
            FAN = OFF;
            TMR2ON = 0;

            // Display message
            clear_screen();
            clcd_print("DOOR IS OPENED", LINE1(0));
            clcd_print("Please close door", LINE2(0));

            unsigned int counter = 0;

            while (RB0 == 0) { // While button is still pressed
                __delay_ms(100);
                counter++;

                if (counter >= 20) { // Long press detected (~2 seconds)
                    BUZZER = ON; // Turn buzzer ON
                }
            }

            BUZZER = OFF; // Turn buzzer OFF after button released

            // Restore display
            clear_screen();
            clcd_print("Function:", LINE1(0));
            clcd_print("Time", LINE2(0));
            clcd_print("5:START 6:PAUSE", LINE3(0));

            FAN = OFF;
            TMR2ON = 1;
        }
    }
}
