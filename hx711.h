/* CC3200 Library for digital weight scale of hx711
 *
 */

#ifndef HX711_H_
#define HX711_H_

// Pinout to HX711
#define DTA_GPIO_08                  	8       /* P63 - Data Input scale A */
#define CLKB_GPIO_30                  	30      /* P53 - Clock Output scale A */
#define DTB_GPIO_07                    	7       /* P62 - Data Input scale B */
#define CLKB_GPIO_00                    0       /* P50 - Clock Output scale B */

extern int Hx711(void);
long getValue(void);
long averageValue(int times);
void setOffset(long offset);
void setScale(float scale);
extern float getGram(int times);


const int _pin_dout;
const int _pin_slk;
long _offset;
float _scale;

#endif /* HX711_H_ */
