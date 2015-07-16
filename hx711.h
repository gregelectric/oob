/* CC3200 Library for digital weight scale of hx711
 *
 */

#ifndef HX711_H_
#define HX711_H_

// Driverlib includes
#include "hw_types.h"
#include "interrupt.h"
#include "utils.h"
#include "rom.h"
#include "rom_map.h"
#include "pin.h"

// Common interface includes
//#include "gpio_if.h"
//#include "uart_if.h"
//#include "i2c_if.h"
//#include "common.h"

// Common interface includes
#include "gpio_if.h"
#include "pinmux.h"

// Pinout to HX711
#define DT_GPIO_07                    	7       /* P62 - Data Input scale B */
#define CLK_GPIO_00                    0       /* P50 - Clock Output scale B */

void Hx711(long * data);
void getGram(int times, long * data);
void getValue(long * data);
void averageValue(int times, long * data);
void setOffsetA(long offset);
void setScaleA(float scale);
void setOffsetB(long offset);
void setScaleB(float scale);

long _offsetA;
float _scaleA;
long _offsetB;
float _scaleB;
#endif /* HX711_H_ */
