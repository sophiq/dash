#ifndef __IDEL_H
#define __IDEL_H	 
#include "fm33lg0xx_fl.h"
#include "main.h"

//	{GPIOC, FL_GPIO_PIN_0, 0},	 /*位置 PC0    */
//	{GPIOC, FL_GPIO_PIN_1, 0},	 /*备用 PC1    */
//	{GPIOC, FL_GPIO_PIN_6, 0},	 /*按键 PC6    */
//	{GPIOB, FL_GPIO_PIN_7, 0},	 /*大灯 PB7    */
//	{GPIOB, FL_GPIO_PIN_8, 0},	 /*右灯 PB8 */	
//	{GPIOB, FL_GPIO_PIN_9, 0},	 /*左灯 PB9 */

#define POSITION  0
    


void Idle(void);
#endif
