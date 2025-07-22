#include <xc.h>
#include "clcd.h"
#include "digital_keypad.h"
#include "timers.h"
#include "newmain.h"
#include "washing_machine_header.h"

#define _XTAL_FREQ 20000000 

// CONFIG
#pragma config WDTE = OFF  // Watchdog Timer Disabled

unsigned char operational_mode = WASHING_PROGRAM_SCREEN;
unsigned char reset_flag = RESET_WASH_PROGRAM_SCREEN;

// Function to initialize configuration
static void init_config(void) {
    // Initialize CLCD
    init_clcd();

    // Initialize digital keypad
    init_digital_keypad();

    // Initialize timer
    init_timer2();

    // Enable global and peripheral interrupts
    GIE = 1;
    PEIE = 1;
    BUZZER_DDR=0;
    FAN_DDR=0;
    BUZZER=OFF;
    FAN=OFF;
}

void main(void) {
    init_config();
    unsigned char key;

    // Step 1: Show Power On Screen
    clcd_print("Press Key5 to", LINE1(1));
    clcd_print("Power On", LINE2(5));
    clcd_print("Washing Machine", LINE3(1));

    // Wait for SW5 press
    while (read_digital_keypad(STATE) != SW5)
    {

    // Delay and clear
    for (unsigned long int delay = 3000; delay--;);
    }
    clear_screen();

    // Show power on animation
    power_on_screen();

    // Set first operational mode
    //operational_mode = WASHING_PROGRAM_SCREEN;
    //reset_flag = RESET_WASH_PROGRAM_SCREEN;

    // Main loop
    while (1) {
        key = read_digital_keypad(STATE);
        for (unsigned int delay = 3000; delay--;); // debouncing

        // Handle transitions
        if ((key == LPSW4) && (operational_mode == WASHING_PROGRAM_SCREEN)) {
            operational_mode = WATER_LEVEL_SCREEN;
            reset_flag = RESET_WATER_LEVEL_SCREEN;
        }
        else if (key == LPSW4 && operational_mode == WATER_LEVEL_SCREEN) {
            operational_mode = START_STOP_SCREEN;
            clear_screen();
            clcd_print("Press Switch", LINE1(0));
            clcd_print("SW5: START", LINE2(0));
            clcd_print("SW6: STOP", LINE3(0));
            set_time();
        }
        else if(key==SW5 && operational_mode==PAUSE)
        {
            operational_mode=START_SCREEN;
            TMR2ON=ON;
            FAN=ON;
            
        }

        // Handle logic based on current screen
        switch (operational_mode) {
            case WASHING_PROGRAM_SCREEN:
                washing_program_display(key);
                break;

            case WATER_LEVEL_SCREEN:
                water_level_screen(key);
                break;

            case START_STOP_SCREEN:
            {
                if (key == SW5) {
                    operational_mode = START_SCREEN;
                    reset_flag = RESET_START_SCREEN;
                }
                else if (key == SW6) {
                    operational_mode = WASHING_PROGRAM_SCREEN;
                    reset_flag = RESET_WASH_PROGRAM_SCREEN;
                }
            }
                break;

            case START_SCREEN:
                run_program(key);
                break;

            default:
                break;
        }
    }
}
