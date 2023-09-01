/**
 * ----------------------------------------------------------------------------
 * main.c
 * Author: Carl Larsson
 * Description: LED chaser
 * Date: 2023-08-30
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
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
    volatile uint32_t i;
    volatile uint32_t k;

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Check if the peripheral access is enabled.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
        ;
    }
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
        ;
    }

    // Configure the device pins.
    PinoutSet(true, false);

    // Initialize the button driver.
    ButtonsInit();

    // Enable the GPIO pin for the LED (PN0).
    // Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    // Loop forever.
    while(1)
    {
        // Go in ascending order, lighting on LEDs in sequence
        for(k = 1; k < 9; k=k*2)
        {
            // turn on LED, LEDWrite requires the same value (ui32LEDValue) as the LED macro value (ui32LEDMask) to turn on
            LEDWrite(k, k);

            // Busy wait to simulate a delay
            for(i = 0; i < 200000; i++)
            {
                ;
            }

            // Turn off LED
            LEDWrite(k, 0);
        }


        // Go back in descending order, lighting on LEDs in sequence
        for(k = 4; k > 1; k=k/2)
        {
            // turn on LED, LEDWrite requires the same value (ui32LEDValue) as the LED macro value (ui32LEDMask) to turn on
            LEDWrite(k, k);

            // Busy wait to simulate a delay
            for(i = 0; i < 200000; i++)
            {
                ;
            }

            // Turn off LED
            LEDWrite(k, 0);
        }

    }
}
