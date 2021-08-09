/*
 * WS2812.h
 *
 *  Created on: Jul 17, 2021
 *      Author: bradleyschulz
 *
 *  Based on guide from
 *  	https://controllerstech.com/interface-ws2812-with-stm32/
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

// CUSTOMIZE THESE DEFINITIONS
#define NUM_LEDS 10
#define LOW_TIME 13
#define HIGH_TIME 26
#define LED_CHANNEL TIM_CHANNEL_2

// END CUSTOM DEFINITIONS
#define RESET_LENGTH 50
#define BUFFER_LENGTH (24*NUM_LEDS + RESET_LENGTH)
#define PI 3.14159265


void setLED(uint32_t LED, uint8_t Red, uint8_t Green, uint8_t Blue);
void clearLEDs();
void setBrightness(uint8_t brightness);
void updateWS2812();

#endif /* INC_WS2812_H_ */
