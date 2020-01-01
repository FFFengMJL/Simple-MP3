#include "led.h"


//初始化PB1为输出.并使能时钟	    
//LED IO初始化
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

		__HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();	
	
	
		__HAL_AFIO_REMAP_SWJ_NOJTAG();
	
		GPIO_Initure.Pin=GPIO_PIN_15;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);	
   
}
