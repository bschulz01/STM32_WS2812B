# STM32 WS2812B Drivers
This library provides a simple interface to control WS2812B LEDs with STM32 MCUs.

## Configuring the .ioc File
NOTE: I have only been able to get this to work with Advanced-control timers.
Timers that I have tested are:
- STM32F303RET6 Timers 1 and 8

First, check the clock frequencies in the "Clock Configuration" tab.
If the frequency for the ABP timer(s) is 8Mhz or less, it should be increased.
Increase the frequencies by selecting "PLLCLK" in the system clock mux,
as this source can multiply the frequency to increase the timer clock rates.

Return to the "Pinout and Configuration" tab to turn on the timers.
Go to the "Timers" tab and select one of the advanced-control timers.
(Usually, at least timer 1 is an advanced control timer)
Select the channel you would like to use, and set it to "PWM Output CHx".

In the "Parameter Settings" tab, set the counter period (aka the AutoReload Regster).
To get the correct value to set, multiply the clock frequency (in MHz) by 1.25.
For example, for a clock frequency of 32 MHz, the counter period should be 40.
This achieves a period of 1.25 us, which is necessary for WS2812(B) LEDs.

Now, go to the DMA Settings tab and add a DMA request for the configured channel.
Set the direction to "Memory to Peripheral" and ensure the data width is "Half word".
Feel free to adjust the priority if arbitration between events will be necessary.

Now the code is ready to be generated.

## Adjusting The Library
In main.h and main.c, a function to access the correct timer must be added.
In main.h, add the following function declaration in the user private defines section:
```TIM_HandleTypeDef* getLEDTimer();```

Then, implement the function in the User Code 4 section:
```TIM_HandleTypeDef* getLEDTimer() { return &htimX; }```


Where `htimX` corresponds to the timer configured in the .ioc file (ex: `htim1`)

Next, configure the correct high and low times in WS2812B.h.
The WS2812B LEDs register a `0` bit when the signal is high for 33% of the period,
and a `1` when the signal is high for 66% of the period.
So `LOW_TIME` should be equal to the counter period multiplied by 0.33,
and `HIGH_TIME` should be equal to the counter period multiplied by 0.66.

Then define the channel of the timer using the `LED_CHANNEL` defition.
This should correspond to the channel of the timer that was set in the .ioc file.
For example, for timer 1 channel 2, this should be set to `TIM_CHANNEL_2`.

Lastly, define the length of the LED strip with the `NUM_LEDS` value.

Now, add `#include "WS2812B.h"` in the User Includes section in main.c.

## API Overview
Use the `setLED(uint32_t LED, uint8_t Red, uint8_t Green, uint8_t Blue)`
function to set an individual LED's RGB values.
Red, green, and blue values can be set with 8-but precision (0-255).
After setting the correct LEDs, call `updateWS2812()` to update the LED strip.
Note that an LED will retain it's color unless explicitly overwritten.

The LEDs can all be cleared by calling `clearLEDs()` followed by `WS2812()`.

## Example Code
The code present here corresponds to using Timer 1 channel 2 with a 32 MHz clock
to control an LED strip with 10 WS2812B LEDs.
The code in main.c flashes the 10 LEDs on and off with a range of colors.