#ifndef __OLED_H
#define __OLED_H
#include "sys.h"

//OLED¿ØÖÆÓÃº¯Êý
void WriteCmd(u8 command);
void WriteDat(u8 data);
	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 mode);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);

void fill_picture(unsigned char fill_Data);
void picture_1(void);
void picture_2(void);
void OLED_Test(void);



#endif
