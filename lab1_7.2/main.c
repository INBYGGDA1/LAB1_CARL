/**
 * ----------------------------------------------------------------------------
 * main.c
 * Author: Carl Larsson
 * Description: Right button acts as an ON/OFF button for LED. Left button has the LED turned ON while it's pressed and it turns OFF when it is no longer pressed.
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

        // Right button acts as an ON/OFF button
        if(BUTTON_PRESSED(RIGHT_BUTTON, ucState, ucDelta))
        {
            // Switch the state of the LED
            LEDWrite(CLP_D1, !GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_1));
        }

        // Left button has the LED turned ON while it's pressed and it turns OFF when it is no longer pressed
        if(BUTTON_PRESSED(LEFT_BUTTON, ucState, ucDelta))
        {
            // Turn on D1.
            LEDWrite(CLP_D1, CLP_D1);
        }
        // Left button has the LED turned OFF when it is no longer pressed
        if(BUTTON_RELEASED(LEFT_BUTTON, ucState, ucDelta))
        {
            // To turn off the LED once the left button is released
            LEDWrite(CLP_D1, 0);
        }
    }
}
//=============================================================================
