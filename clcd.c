#define _XTAL_FREQ 20000000
#include <xc.h>

#include "clcd.h"
#include "newmain.h"

// Write a byte to CLCD
void clcd_write(unsigned char byte, unsigned char mode)
{
    CLCD_RS = mode;
    CLCD_DATA_PORT = byte;
    
    CLCD_EN = HI;
    __delay_us(100);   // Min delay to latch
    CLCD_EN = LOW;
    
    __delay_us(4100);  // Command delay
}

// Internal function to initialize the CLCD controller
static void init_display_controller(void)
{
    __delay_ms(30);    // Startup delay

    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(4100);
    
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(100);
    
    clcd_write(EIGHT_BIT_MODE, INST_MODE);
    __delay_us(1); 

    clcd_write(TWO_LINES_5x8_8_BIT_MODE, INST_MODE);
    __delay_us(100);

    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_ms(2); // Safer to use ms delay for clear

    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    __delay_us(100);
}

// Public function to initialize CLCD
void init_clcd(void)
{
    CLCD_DATA_PORT_DDR = 0x00;   // Set entire data port as output
    CLCD_RS_DDR = 0;             // RS pin as output
    CLCD_EN_DDR = 0;             // EN pin as output

    init_display_controller();   // Execute controller initialization
}

// Print single character to CLCD
void clcd_putch(const char data, unsigned char addr)
{
    clcd_write(addr, INST_MODE); // Set DDRAM address
    clcd_write(data, DATA_MODE); // Write data
}

// Print a string starting from given address
void clcd_print(const char *str, unsigned char addr)
{
    clcd_write(addr, INST_MODE);
    
    while (*str != '\0')
    {
        clcd_write(*str++, DATA_MODE);
    }
}
