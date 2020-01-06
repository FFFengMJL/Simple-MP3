#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "myiic.h"


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
	OLED_ShowString(0,0, "0.96' OLED TEST",16);  
//	OLED_DrawPoint(127, 0, 1);
//	OLED_DrawPoint(126, 0, 1);
	for (t=0; t<128;t++)
		OLED_DrawPoint(t, 0, 1);
	
//	OLED_ShowChar(0,0,'B',24,1);
// 	OLED_ShowString(0,40,"DATE 2018/5/04",12);  
// 	OLED_ShowString(0,52,"ASCII:",12);  
// 	OLED_ShowString(64,52,"CODE:",12);  
//	OLED_Fill(10,0,30,30,1);
	OLED_Refresh_Gram();		//?????????OLED 
	 //picture_1();
	//picture_1();

	//OLED_ShowString(10,10,"i love you", 12);
 
	
	while(1) 
	{			
		LED=!LED;
		//OLED_Test();	
	}	
	
}

