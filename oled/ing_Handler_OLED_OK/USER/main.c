#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "myiic.h"

void showStatus(int playing, int song) {
	int i;
	int nextSongList[] = {16, 7, 8, 9};
	
	OLED_Clear();
	for (i = 0; i < 128; i++) {
		OLED_DrawPoint(i, 0, 1);
		OLED_DrawPoint(i, 63, 1);
	}
	for (i = 1; i < 63; i++) {
		OLED_DrawPoint(0, i, 1);
		OLED_DrawPoint(127, i, 1);
	}
  if (playing == 1) {
		int charList[] = {0, 1, 2, 3};
		int controlList[] = {5, 6, 15};
		for (i = 0; i < 4; i++) {
		  OLED_ShowChinese(32 + 16 * i, 1, charList[i], 16);
		}
		for (i = 0; i < 3; i++) {
			OLED_ShowChinese(1 + 16 * i, 47, controlList[i], 16);
		}
	} else {
		int charList[] = {4, 5, 6, 13};
		int controlList[] = {2, 3, 14};
		for (i = 0; i < 4; i++) {
		  OLED_ShowChinese(32 + 16 * i, 1, charList[i], 16);
		}
		for (i = 0; i < 3; i++) {
			OLED_ShowChinese(1 + 16 * i, 47, controlList[i], 16);
		}
	}
	if (song == 1) {
		int songNameList[] = {10, 11, 12, 13};
		for (i = 0; i < 4; i++) {
			OLED_ShowChinese(32 + 16 * i, 24, songNameList[i], 16);
		}
	}
	for (i = 0; i < 4; i++) {
		OLED_ShowChinese(127 - 16 * (4 - i), 47, nextSongList[i], 16);
	}
	OLED_Refresh_Gram();
}

int main(void)
{
	u8 t=0; 
	HAL_Init();                    	 	//?????HAL??    
	Stm32_Clock_Init(RCC_PLL_MUL6);   	//???????,72M
	delay_init(72);               		//????????????
	uart_init(115200);					//?????????
	LED_Init();							//?????LED	

	IIC_Init();
	OLED_Init();						//?????OLED      
	
	
//	OLED_ShowString(0,0, "120",24);  
	// OLED_ShowString(0,0, "0.96' OLED TEST",16);  
	//OLED_ShowChinese(0, 0, 0, 16);
	//OLED_ShowChinese(16, 0, 1, 16);
//	OLED_DrawPoint(127, 0, 1);
//	OLED_DrawPoint(126, 0, 1);
	
//	OLED_ShowChar(0,0,'B',24,1);
// 	OLED_ShowString(0,40,"DATE 2018/5/04",12);  
// 	OLED_ShowString(0,52,"ASCII:",12);  
// 	OLED_ShowString(64,52,"CODE:",12);  
//	OLED_Fill(10,0,30,30,1);
	//OLED_Refresh_Gram();		//?????????OLED 
	 //picture_1();
	//picture_1();

	//OLED_ShowString(10,10,"i love you", 12);
	showStatus(1, 1);
 
	
	while(1) 
	{			
		LED=!LED;
		//OLED_Test();	
	}	
	
}
