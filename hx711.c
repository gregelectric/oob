// Driverlib includes
#include "hw_types.h"
#include "interrupt.h"
#include "utils.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"

// Common interface includes
#include "gpio_if.h"
#include "hx711.h"
#include "gpio_if.h"
#include "pinmux.h"

int Hx711()
{
    unsigned int uiGPIOPort;
    unsigned char pucGPIOPin;
    int tare;

    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

    MAP_UtilsDelay((1)); // ?

    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

	tare = averageValue(32);
	setOffset(tare);
	setScale(742);
	return tare;
}

long averageValue(int times)
{
	int i;
	long sum = 0;

	for (i = 0; i < times; i++)
	{
		sum += getValue();
	}

	return sum / times;
}

long getValue()
{
    unsigned int uiGPIOPort;
    unsigned char pucGPIOPin;
    unsigned char ucPinValue;
	int data[3];
	int j;
	int i;

	//Read GPIO
	GPIO_IF_GetPortNPin(DTB_GPIO_07,&uiGPIOPort,&pucGPIOPin);
	while(GPIO_IF_Get(DTB_GPIO_07,uiGPIOPort,pucGPIOPin))
	{
		for (j = 3; j--;)
		{
			for (i = 8; i--;)
			{
			    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
			    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

			    //Read GPIO
			    GPIO_IF_GetPortNPin(DTB_GPIO_07,&uiGPIOPort,&pucGPIOPin);
			    ucPinValue = GPIO_IF_Get(DTB_GPIO_07,uiGPIOPort,pucGPIOPin);

			    //If Connected to VCC, Mode is AP
			    if(ucPinValue == 1)
			    {
			    	data[j] = 1;
			    }
			    else
			    {
			    	data[j] = 0;
			    }

			    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
			    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 0);
			}
		}
	}

    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 1);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

	data[2] ^= 0x80;

	return ((long) data[2] << 16) | ((long) data[1] << 8)
			| (long) data[0];
}

void setOffset(long offset)
{
	_offset = offset;
}

void setScale(float scale)
{
	_scale = scale;
}

float getGram(int times)
{
	long val = (averageValue(times) - _offset);
	return (float) val / _scale;
}
