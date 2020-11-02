#include "daisy_petal.h"
#include "terrarium.h"

using namespace daisy;
using namespace terrarium;

DaisyPetal hardware;

Parameter wait_time;

dsy_gpio left_led;
int left_led_pin = 22;
int left_led_state = false;
dsy_gpio right_led;
int right_led_pin = 23;
int right_led_state = false;

bool left_footswitch_pressed;
bool right_footswitch_pressed;
bool switch_one_pressed;
bool switch_two_pressed;
bool switch_three_pressed;
bool switch_four_pressed;

int t;


void callback(float *in, float *out, size_t size)
{
    hardware.DebounceControls();
    hardware.UpdateAnalogControls();

    wait_time.Process();
    for (size_t i = 0; i < size; i+=2)
        out[i]     = in[i];
        out[i + 1] = in[i + 1];
}

int main(void)
{
    bool pcb_led_state;
    pcb_led_state = true;

    hardware.Init();
    hardware.StartAdc();
    hardware.StartAudio(callback);

    wait_time.Init(hardware.knob[Terrarium::KNOB_1], 0.0f, 1000.0f, Parameter::LINEAR);

    left_led.pin = hardware.seed.GetPin(left_led_pin);
    left_led.mode = DSY_GPIO_MODE_OUTPUT_PP;
    left_led.pull = DSY_GPIO_NOPULL;
    dsy_gpio_init(&left_led);

    right_led.pin = hardware.seed.GetPin(right_led_pin);
    right_led.mode = DSY_GPIO_MODE_OUTPUT_PP;
    left_led.pull = DSY_GPIO_NOPULL; 
    dsy_gpio_init(&right_led);
    dsy_gpio_toggle(&right_led);

    for(;;)
    {
        hardware.seed.SetLed(pcb_led_state);
        dsy_gpio_write(&left_led, left_led_state);
        dsy_gpio_write(&right_led, right_led_state);

        hardware.DebounceControls();
        hardware.UpdateAnalogControls();

        left_footswitch_pressed = hardware.switches[Terrarium::FOOTSWITCH_1].Pressed();
        right_footswitch_pressed = hardware.switches[Terrarium::FOOTSWITCH_2].Pressed();

        switch_one_pressed = hardware.switches[Terrarium::SWITCH_1].Pressed() || hardware.switches[Terrarium::SWITCH_1].RisingEdge();
        switch_two_pressed = hardware.switches[Terrarium::SWITCH_2].Pressed() || hardware.switches[Terrarium::SWITCH_2].RisingEdge();
        switch_three_pressed = hardware.switches[Terrarium::SWITCH_3].Pressed() || hardware.switches[Terrarium::SWITCH_3].RisingEdge();
        switch_four_pressed = hardware.switches[Terrarium::SWITCH_4].Pressed() || hardware.switches[Terrarium::SWITCH_4].RisingEdge();

        if (left_footswitch_pressed) {
            left_led_state = true;
        } else {
            if (switch_one_pressed && switch_two_pressed) {
                left_led_state = !left_led_state;
            } else {
                left_led_state = false;
            }
        }

        if (right_footswitch_pressed) {
            right_led_state = true;
        } else {
            if (switch_three_pressed && switch_four_pressed) {
                right_led_state = !right_led_state;
            } else {
                right_led_state = false;
            }
        }

        pcb_led_state = !pcb_led_state;

        t = 1000 - (int)wait_time.Value();
        dsy_system_delay(t);
    }
}
