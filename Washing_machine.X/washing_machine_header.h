/* 
 * File:   washing_machine_header.h
 * Author: hp
 *
 * Created on 26 June, 2025, 9:32 PM
 */

#ifndef WASHING_MACHINE_HEADER_H
#define	WASHING_MACHINE_HEADER_H

void power_on_screen(void);
void clear_screen(void);
void washing_program_display(unsigned char key);
void water_level_screen( unsigned char key);
void set_time(void);
void run_program(unsigned char key);
void door_status_check(void);
#endif	/* WASHING_MACHINE_HEADER_H */

