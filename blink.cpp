#include "daisy_petal.h"
#include "terrarium.h"

// Use the daisy namespace to prevent having to type 
// daisy:: before all libdaisy functions
using namespace daisy;
// Use the terrarium namespace to prevent having to type
// terrarium:: before all terrarium switch constants
using namespace terrarium;

// Declare a DaisySeed object called hardware
DaisyPetal hardware;

dsy_gpio led1;
dsy_gpio led2;

int main(void)
{
    // Declare a variable to store the state we want to set for the LED.
    bool led_state;
    led_state = true;

    // Configure and Initialize the Terrarium
    hardware.Init();

    led1.pin = hardware.seed.GetPin(22);
    led1.mode = DSY_GPIO_MODE_OUTPUT_PP;
    led1.pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&led1);

    led2.pin = hardware.seed.GetPin(23);
    led2.mode = DSY_GPIO_MODE_OUTPUT_PP;
    led1.pull = DSY_GPIO_NOPULL; 
    dsy_gpio_init(&led2);
    dsy_gpio_toggle(&led2);

    // Loop forever
    for(;;)
    {
        // Set the onboard LED
        hardware.seed.SetLed(led_state);
        
        // Toggle the LED state for the next time around.
        hardware.DebounceControls();
        hardware.UpdateAnalogControls();

        if (hardware.switches[Terrarium::FOOTSWITCH_1].Pressed()) {

            dsy_gpio_write(&led1, true);

        } else {

            if (hardware.switches[Terrarium::SWITCH_1].Pressed() or hardware.switches[Terrarium::SWITCH_1].RisingEdge()) {
                dsy_gpio_toggle(&led1);
            } else if (!hardware.switches[Terrarium::SWITCH_1].Pressed() or hardware.switches[Terrarium::SWITCH_1].FallingEdge()) {
                dsy_gpio_write(&led1, false);
            }

        }

        hardware.DebounceControls();
        hardware.UpdateAnalogControls();

        if (hardware.switches[Terrarium::SWITCH_2].Pressed() or hardware.switches[Terrarium::SWITCH_2].RisingEdge()) {
            dsy_gpio_toggle(&led2);
        } else if (!hardware.switches[Terrarium::SWITCH_2].Pressed() or hardware.switches[Terrarium::SWITCH_2].FallingEdge()) {
            dsy_gpio_write(&led2, false);
        }

        led_state = !led_state;

        dsy_system_delay(100);
    }
}
