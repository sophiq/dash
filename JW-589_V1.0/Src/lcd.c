#include "lcd.h"
#include "user_init.h"

#define START_1 GPIOA->DSET=0x40
#define START_0 GPIOA->DRST=0x40
//#define LCD_CLK_H  GPIOA->DSET |= 0x1000;
//#define LCD_CLK_L  GPIOA->DRST |= 0x1000;
//#define LCD_A0_H   GPIOB->DSET |= 0x0008;
//#define LCD_A0_L   GPIOB->DRST |= 0x0008;
//#define LCD_CS_H   GPIOB->DSET |= 0x0010;
//#define LCD_CS_L   GPIOB->DRST |= 0x0010;
//#define LCD_RST_H  GPIOB->DSET |= 0x0020;
//#define LCD_RST_L  GPIOB->DRST |= 0x0020;
//#define LCD_Dout_H GPIOB->DSET |= 0x0004;
//#define LCD_Dout_L GPIOB->DRST |= 0x0004;




//±³¹â  PA5
//CLK   PB4
//A0    PB6
//CS    PB7
//RST   PB8
//DOUT  PB5

//#define BACKLIGHT_ON   GPIOA->DSET |= 0x0020;   
//#define BACKLIGHT_OFF  GPIOA->DRST |= 0x0020;


#define LCD_CLK_H  GPIOB->DSET |= 0x0010;   
#define LCD_CLK_L  GPIOB->DRST |= 0x0010;
#define LCD_A0_H   GPIOB->DSET |= 0x0040;
#define LCD_A0_L   GPIOB->DRST |= 0x0040;
#define LCD_CS_H   GPIOB->DSET |= 0x0080;
#define LCD_CS_L   GPIOB->DRST |= 0x0080;
#define LCD_RST_H  GPIOB->DSET |= 0x0100;
#define LCD_RST_L  GPIOB->DRST |= 0x0100;
#define LCD_Dout_H GPIOB->DSET |= 0x0020;
#define LCD_Dout_L GPIOB->DRST |= 0x0020;

/*!-----------------------------------------------------------------
 */
#define ins_nop()							wrins(0x00)			/*!< No Operation */
#define ins_software_reset()				wrins(0x01)			/*!< Software reset */
#define ins_sleep_in()						wrins(0x10)			/*!< Sleep in mode */
#define ins_sleep_out()						wrins(0x11)			/*!< Sleep out mode */
#define ins_inverse_display_off()			wrins(0x20)			/*!< Display Inversion off */
#define ins_inverse_display_on()			wrins(0x21)			/*!< Display Inversion on */
#define ins_exit_all_point_on()				wrins(0x22)			/*!< Exit all point on */
#define ins_enter_all_point_on()			wrins(0x23)			/*!< Enter all point on */
#define ins_display_off()					wrins(0x28)			/*!< Display off */
#define ins_display_on()					wrins(0x29)			/*!< Display on */
#define ins_column_address_set(start, end)	do{					\
												wrins(0x2A);	\
												wrdata((start));\
												wrdata((end));	\
											}while(0)			/*!< Column address set */
#define ins_row_address_set(start, end)		do{					\
												wrins(0x2B);	\
												wrdata((start));\
												wrdata((end));	\
											}while(0)			/*!< Row address set */
#define ins_memory_write()					wrins(0x2C)			/*!< Write data to memory */
#define ins_memory_read()					wrins(0x2E)			/*!< Read data from memory */
#define ins_duty_set(duty)					do{					\
												wrins(0xB0);	\
												wrdata((duty));	\
											}while(0)			/*!< Duty set Range 1 to 4 duty */
#define ins_frame_Frequency(freq)			do{					\
												wrins(0xB2);	\
												wrdata((freq));	\
											}while(0)			/*!< Frame Frequency */
#define ins_lcd_scan_set(scan)				do{					\
												wrins(0xB7);	\
												wrdata((scan));	\
											}while(0)			/*!< LCD scan set */
#define ins_enter_read_modify()				wrins(0xB8)			/*!< Enter Read modify */
#define ins_exit_read_modify()				wrins(0xB9)			/*!< Exit Read modify */
#define ins_vop_set(h, l)					do{					\
											wrins(0xC0);		\
											wrdata((l));		\
											wrdata((h));		\
											}while(0)			/*!< Vop set Range 3V to 13.2V; V0 = 3 + [l * 255 + h] x 0.04 */
#define ins_bias_selection(bias)			do{					\
												wrins(0xC3);	\
												wrdata((bias));	\
											}while(0)			/*!< Bias selection */
#define ins_power_Control(x)				do{					\
												wrins(0xD2);	\
												wrdata((x));	\
											}while(0)			/*!< Power Control */
#define dat_memory_write(dat)				wrdata((dat))		/*!< Write data to memory */


bool GetBit(uint8_t n, uint8_t k)
{
    bool bx;

    if(((n >> k) & 1) == 1)
        bx = 1;
    else
        bx = 0;
    return bx;
}



static void lcdgpioinit(void)
{
	FL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.pin = FL_GPIO_PIN_4 | FL_GPIO_PIN_5 | FL_GPIO_PIN_6 | FL_GPIO_PIN_7 | FL_GPIO_PIN_8;
	GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull = FL_DISABLE;
	FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	LCD_RST_H
	LCD_RST_L
	FL_DelayMs(20);
	LCD_RST_H
}




static void wrins(uint8_t ins)
{
	uint8_t i = 0;
	LCD_CS_H
	LCD_A0_L
	LCD_CS_L
	for(i = 0; i < 8; i++){
		LCD_CLK_L
		if(((ins<<i)&0x80) == 0x80)LCD_Dout_H
    else                       LCD_Dout_L			
		LCD_CLK_H
	}
	LCD_CS_H
}

static void wrdata(uint8_t data)
{
	uint8_t i = 0;
	LCD_CS_H
	LCD_A0_H
	LCD_CS_L
	for(i = 0; i < 8; i++){
		LCD_CLK_L
		if(((data<<i)&0x80) == 0x80)LCD_Dout_H
    else                        LCD_Dout_L			
		LCD_CLK_H
	}
	LCD_CS_H
}

void init(void)
{
	lcdgpioinit();
	FL_DelayMs(20);
	wrins(0x11);		//
	FL_DelayMs(20);
	wrins(0xd2);		//
	wrdata(0x00);		// 
	FL_DelayMs(20);
	wrins(0xc0);		//
	wrdata(0x52);		// 
	wrdata(0x00);		//
	wrins(0xc3);		//
	wrdata(0x01);		// 
  wrins(0xB0);		//
	wrdata(0x01);		// 
	wrins(0x2A);		//
	wrdata(0x00);		// 
	wrdata(0x9F);		//  
	wrins(0xB7);		//
	wrdata(0x00);		//
	wrins(0x2B);		//
	wrdata(0x00);		// 
	wrdata(0x01);		//  
	wrins(0xB2);		//
	wrdata(0x1d);		//        
	wrdata(0x29);
}
void LcdConfig(void)
{
	lcdgpioinit();
	FL_DelayMs(20);
	ins_sleep_out();											               // sleep out 0x11
	FL_DelayMs(20);
	ins_power_Control(0x00);									           // set power control [all power on] 0xD2
	FL_DelayMs(20);
	ins_vop_set(0x00,0x52);									           // set Vop [3.48V] 0xC0
	ins_bias_selection(E_BIAS_1DIV3);										 // set Bias [1/2]  0xC3
	ins_duty_set(E_DUTY_1DIV2);									         // set Duty [1/1]	0xB0
	ins_column_address_set(0x00, 0x9F); // set SEG address	0x2A
	wrins(0xB7);		
	wrdata(0X00);	
	ins_row_address_set(0x00, 0x01);	 	 // set COM address 0x2B
	ins_frame_Frequency(E_FREQ_140HZ);					 		     // set frame frequency [140Hz] 0xB2
	ins_display_on();											               // Display on	
}

void SC5037_ClearAllPoints(void)
{
	uint8_t x, y;
	
	ins_column_address_set(0x00, Y_POSITION_NUMBER - 1);
	ins_row_address_set(0x00, X_POSITION_NUMBER - 1);
	ins_memory_write();
	for(x = 0; x < X_POSITION_NUMBER; x++){
		for(y = 0; y < Y_POSITION_NUMBER; y++){
			dat_memory_write(V_DISP_OFF_H | V_DISP_OFF_L);
		}
	}
}

void SC5037_DisplayAll(uint8_t on)
{
	if(on) ins_enter_all_point_on();
	else   ins_exit_all_point_on();
}







uint8_t thisgraphic[]={

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x07,0x00,0x00,0x00,0x00,//1-1 S31/S30
0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,//1-2 S63/S62
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,//1-3 S95/S94
0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,//1-4 S127/S126
0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,//1-5 S159/S158
0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,//1-6 S191/S190
0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x07,//1-7 S223/S222
0x00,0x00,0x00,0x70,0x00,0x00,0x70,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x07,0x70,//1-8 S255/S254
0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,//1-9 S287/S286
0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1-10S319/S318

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x07,0x00,0x00,0x00,0x00,//1-1 S31/S30
0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x70,0x00,//1-2 S63/S62
0x07,0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,//1-3 S95/S94
0x70,0x00,0x07,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,//1-4 S127/S126
0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x07,0x70,0x00,//1-5 S159/S158
0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x07,0x00,0x00,0x00,0x00,0x70,0x00,//1-6 S191/S190
0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,//1-7 S223/S222
0x00,0x00,0x00,0x70,0x00,0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x70,0x00,0x07,0x00,//1-8 S255/S254
0x00,0x00,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x07,0x70,0x00,0x00,0x00,0x00,//1-9 S287/S286
0x07,0x00,0x00,0x00,0x70,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1-10S319/S318
};
void dispgraphic(uint8_t graphic[])
{
 int i,j,k;						  ///////////
 k=0;
 for (i=0;i<1;i++)    //8
	 {
			wrins(0x2a);		//
			wrdata(0x00);		//
			wrdata(0x9f);		//

			wrins(0x2B);		//
			wrdata(0x00);		// 
			wrdata(0x01);		// 

			wrins(0x2c);		//
			for (j=0;j<320;j++)		 //
				{
						
					wrdata(graphic[k]);
					k++;
			 //
				}
			}
		wrins(0x29);
 }
void dsp(void){
	
	dispgraphic(thisgraphic);
	
}







void SC5037_ShowRectangeEx(uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2, uint8_t* pflags, uint8_t len)
{
	static uint8_t idx = 0;
	uint8_t v3_h4, v3_l4, v2_h4, v2_l4, v1_h4, v1_l4, v0_h4, v0_l4;
	
	if(idx == 0){
		ins_row_address_set(0, 1);
		ins_column_address_set(0, 159);
		ins_memory_write();
	}

	if(idx < len){
		v0_l4 = GetBit(*(pflags + idx), 0) ? V_DISP_ON_L : V_DISP_OFF_L;
		v0_h4 = GetBit(*(pflags + idx), 1) ? V_DISP_ON_H : V_DISP_OFF_H;
		v1_l4 = GetBit(*(pflags + idx), 2) ? V_DISP_ON_L : V_DISP_OFF_L;
		v1_h4 = GetBit(*(pflags + idx), 3) ? V_DISP_ON_H : V_DISP_OFF_H;
		v2_l4 = GetBit(*(pflags + idx), 4) ? V_DISP_ON_L : V_DISP_OFF_L;
		v2_h4 = GetBit(*(pflags + idx), 5) ? V_DISP_ON_H : V_DISP_OFF_H;
		v3_l4 = GetBit(*(pflags + idx), 6) ? V_DISP_ON_L : V_DISP_OFF_L;
		v3_h4 = GetBit(*(pflags + idx), 7) ? V_DISP_ON_H : V_DISP_OFF_H;

		dat_memory_write(v0_h4 | v0_l4);
		dat_memory_write(v1_h4 | v1_l4);
		dat_memory_write(v2_h4 | v2_l4);
		dat_memory_write(v3_h4 | v3_l4);
		idx++;
	}
	else{
		idx = 0;
	}
}
//void SC5037_ShowRectangeEx(uint8_t x1, uint8_t x2, uint16_t y1, uint16_t y2, uint8_t* pflags, uint8_t len)
//{
//	static uint8_t idx = 0;
//	uint8_t v3_h4, v3_l4, v2_h4, v2_l4, v1_h4, v1_l4, v0_h4, v0_l4;
//	
//	if(idx == 0){
//		ins_row_address_set(x1, x2);
//		ins_column_address_set(SEG2YPOS(y1), SEG2YPOS(y2));
//		ins_memory_write();
//	}

//	if(idx < len){
//		v0_l4 = GetBit(*(pflags + idx), 0) ? V_DISP_ON_L : V_DISP_OFF_L;
//		v0_h4 = GetBit(*(pflags + idx), 1) ? V_DISP_ON_H : V_DISP_OFF_H;
//		v1_l4 = GetBit(*(pflags + idx), 2) ? V_DISP_ON_L : V_DISP_OFF_L;
//		v1_h4 = GetBit(*(pflags + idx), 3) ? V_DISP_ON_H : V_DISP_OFF_H;
//		v2_l4 = GetBit(*(pflags + idx), 4) ? V_DISP_ON_L : V_DISP_OFF_L;
//		v2_h4 = GetBit(*(pflags + idx), 5) ? V_DISP_ON_H : V_DISP_OFF_H;
//		v3_l4 = GetBit(*(pflags + idx), 6) ? V_DISP_ON_L : V_DISP_OFF_L;
//		v3_h4 = GetBit(*(pflags + idx), 7) ? V_DISP_ON_H : V_DISP_OFF_H;

//		dat_memory_write(v0_h4 | v0_l4);
//		dat_memory_write(v1_h4 | v1_l4);
//		dat_memory_write(v2_h4 | v2_l4);
//		dat_memory_write(v3_h4 | v3_l4);
//		idx++;
//	}
//	else{
//		idx = 0;
//	}
//}
