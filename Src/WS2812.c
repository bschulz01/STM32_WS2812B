/*
 * WS2812.c
 *
 *  Created on: Jul 17, 2021
 *      Author: bradleyschulz
 */


#include "main.h"
#include "WS2812.h"
#include <math.h>



uint8_t LED_Data[NUM_LEDS][3];
uint16_t PWM_Data[BUFFER_LENGTH];

uint8_t dataSentFlag = 0;

// Callback for when data transfer is complete
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(getLEDTimer(), LED_CHANNEL);
	dataSentFlag = 1;
}

void setLED(uint32_t LED, uint8_t Red, uint8_t Green, uint8_t Blue)
{
	LED_Data[LED][0] = Green;
	LED_Data[LED][1] = Red;
	LED_Data[LED][2] = Blue;
}

void clearLEDs()
{
	for (int i = 0; i < NUM_LEDS; i++)
	{
		setLED(i, 0, 0, 0);
	}
}

// Sets the brightness on a scale from 0-45
void setBrightness(uint8_t brightness)
{
	// Truncate brightness
	if (brightness > 45)
	{
		brightness = 45;
	}
	for (uint32_t i = 0; i < NUM_LEDS; i++)
	{
		for (uint8_t j = 0; j < 3; j++)
		{
			float angle = (90 - brightness) * PI / 180; // convert degrees to radians
			LED_Data[i][j] = (LED_Data[i][j]) / tan(angle); // scale brightness
		}
	}
}

void updateWS2812()
{
	uint32_t color;

	uint16_t idx = 0;
	// Load buffer with LED data
	for (uint32_t led = 0; led < NUM_LEDS; led++)
	{
		// Generate bits to describe color
		color = (LED_Data[led][0] << 16) | (LED_Data[led][1] << 8) | (LED_Data[led][2]);

		// Set the buffer for this LED
		// Send MSB to LSB
		for (int16_t bit = 23; bit >= 0; bit--)
		{
			//uint32_t idx = 24 * led + bit;
			if (color & (1<<bit))
			{
				PWM_Data[idx] = HIGH_TIME; // High bit
			}
			else
			{
				PWM_Data[idx] = LOW_TIME; // Low bit
			}
			idx++;
		}
	}

	// Reset the communication line to signify end of transmission
	for (; idx < BUFFER_LENGTH; idx++)
	{
		PWM_Data[idx] = 0;
	}


	// Begin transfer of data
	HAL_TIM_PWM_Start_DMA(getLEDTimer(), LED_CHANNEL, (uint32_t *)PWM_Data, idx);
	while(!dataSentFlag)
		continue;
	dataSentFlag = 0;
}
