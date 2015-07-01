#include "hx711.h"

int Hx711()
{
    unsigned int uiGPIOPort;
    unsigned char pucGPIOPin;
    int tare;

    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

	tare = averageValue(32);
	setOffset(tare);
	setScale(742.f);
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
	long data = 0;
	int i;


	// Wait till HX711 is ready: Data is low
	GPIO_IF_GetPortNPin(DTB_GPIO_07,&uiGPIOPort,&pucGPIOPin);
	while(GPIO_IF_Get(DTB_GPIO_07,uiGPIOPort,pucGPIOPin));

	// Read out 24 bits
	for (i = 0; i<24; i++)
	{

			// Set clock
		    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
		    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

		    // Read Bit
		    GPIO_IF_GetPortNPin(DTB_GPIO_07,&uiGPIOPort,&pucGPIOPin);
		    if(GPIO_IF_Get(DTB_GPIO_07,uiGPIOPort,pucGPIOPin))
		    {
		    	data = (data | 1);
		    }
		    data = (data << 1);

		    // Clear Clock
		    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
		    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 0);
	}

    GPIO_IF_GetPortNPin(CLKB_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 1);
    GPIO_IF_Set(CLKB_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

	//data ^= 0x80;

	return data;
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
