#include "hx711.h"

void Hx711(long * data)
{
    unsigned int uiGPIOPort;
    unsigned char pucGPIOPin;

    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

	averageValue(32, data);
	setOffsetA(data[0]);
	setScaleA(853.3f);
	setOffsetB(data[1]);
	setScaleB(222.6f);
	return;
}

void averageValue(int times, long *data)
{
	int i;
    long   ave[2] = {0, 0};

	for (i = 0; i < times; i++)
	{
		getValue(data);
		ave[0] += data[0];
		ave[1] += data[1];
	}

	data[0] = ave[0] / times;
	data[1] = ave[1] / times;
	return;
}

void getValue(long * data)
{
    unsigned int uiGPIOPort;
    unsigned char pucGPIOPin;
	int i;
	long dataA = 0;
	long dataB = 0;


	// Wait till HX711 is ready: Data is low
	GPIO_IF_GetPortNPin(DT_GPIO_07,&uiGPIOPort,&pucGPIOPin);
	while(GPIO_IF_Get(DT_GPIO_07,uiGPIOPort,pucGPIOPin));

	// Read Channel A Gain 128 - 24 bits
	for (i = 0; i<24; i++)
	{

			// Set clock
		    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
		    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

		    // Read Bit
		    GPIO_IF_GetPortNPin(DT_GPIO_07,&uiGPIOPort,&pucGPIOPin);
		    if(GPIO_IF_Get(DT_GPIO_07,uiGPIOPort,pucGPIOPin))
		    {
		    	dataA = (dataA | 1);
		    }
		    dataA = (dataA << 1);

		    // Clear Clock
		    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
		    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 0);
	}

	// Set next conversionto Channel B Gain 32 - 24 bits
    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 1);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 0);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 1);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

	// Wait till HX711 is ready: Data is low
	GPIO_IF_GetPortNPin(DT_GPIO_07,&uiGPIOPort,&pucGPIOPin);
	while(GPIO_IF_Get(DT_GPIO_07,uiGPIOPort,pucGPIOPin));

	// Read Channel B Gain 32 - 24 bits
	for (i = 0; i<24; i++)
	{

			// Set clock
		    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
		    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 1);

		    // Read Bit
		    GPIO_IF_GetPortNPin(DT_GPIO_07,&uiGPIOPort,&pucGPIOPin);
		    if(GPIO_IF_Get(DT_GPIO_07,uiGPIOPort,pucGPIOPin))
		    {
		    	dataB = (dataB | 1);
		    }
		    dataB = (dataB << 1);

		    // Clear Clock
		    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
		    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 0);
	}

	// Set next conversionto Channel A Gain 128 - 24 bits
    GPIO_IF_GetPortNPin(CLK_GPIO_00,&uiGPIOPort,&pucGPIOPin);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 1);
    GPIO_IF_Set(CLK_GPIO_00, uiGPIOPort, pucGPIOPin, 0);

    data[0] = dataA;
    data[1] = dataB;
    return;
}

void setOffsetA(long offset)
{
	_offsetA = offset;
    return;
}

void setScaleA(float scale)
{
	_scaleA = scale;
    return;
}
void setOffsetB(long offset)
{
	_offsetB = offset;
    return;
}

void setScaleB(float scale)
{
	_scaleB = scale;
    return;
}

void getGram(int times, long * data)
{
	averageValue(times, data);
	data[0] = labs( (data[0] - _offsetA) / _scaleA);
	data[1] = labs( (data[1] - _offsetB) / _scaleB);
	return;
}
