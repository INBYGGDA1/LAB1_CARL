/**
 * ----------------------------------------------------------------------------
 * main.c
 * Author: Carl Larsson
 * Description: LED starts blinking when button has been pressed
 * Date: 2023-08-30
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "drivers/buttons.h"
#include "drivers/pinout.h"

//=============================================================================
// The error routine that is called if the driver library
// encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif
//=============================================================================
// Main function
int main(void)
{
    unsigned char ucDelta, ucState;
    volatile uint32_t i;

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
        ;
    }

    // Configure the device pins.
    PinoutSet(false, false);

    // Initialize the button driver.
    ButtonsInit();

    // Enable the GPIO pin for the LED (PN0).
    // Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    // Loop forever.
    while(1)
    {
        // Poll the buttons.
        ucState = ButtonsPoll(&ucDelta, 0);

        // Blink LED when button has been pressed
        if(BUTTON_PRESSED(RIGHT_BUTTON, ucState, ucDelta))
        {
            while(1)
            {
                // Switch the state of the LED
                LEDWrite(CLP_D1, !GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_1));

                // Busy wait to simulate a delay
                for(i = 0; i < 200000; i++)
                {
                    ;
                }
            }

        }

    }
}
