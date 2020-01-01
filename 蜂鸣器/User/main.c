/********************************************************************************
* @file    main.c
* @author  Oslomayor
* @date    2018-7-12
*
* Wechat Offical Account: CrazyEngineer
* GitHub  : https://github.com/Oslomayor
* CSDN    : https://blog.csdn.net/weixin_42602730
********************************************************************************/  


#include "stm32f10x.h"
#define		key1_pin		  GPIO_Pin_0
#define		key1_GPIO_PORT            GPIOA
#define		key1_GPIO_CLK		  RCC_AHB1Periph_GPIOA
 
 
#define		key2_pin		  GPIO_Pin_13
#define		key2_GPIO_PORT            GPIOC
#define		key2_GPIO_CLK		  RCC_AHB1Periph_GPIOC
 
 
#define		KEY_ON		1
#define		KEY_OFF		0

uint8_t	Key_Scan(GPIO_TypeDef* GPIOx , uint16_t GPIO_Pin )
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)                  //Detect whether the key is pressed
	{
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);     //Loosen the detection
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}

void BEEP_Init ( void ){
	
	GPIO_InitTypeDef GPIO_InitStructure;   //?????????,?????IO
	
	SystemInit ();
	
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );  //??GPIO??
	
	//??GPIOB?IO??
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init ( GPIOB, &GPIO_InitStructure ); //???GPIO
	
}
int melody[] = {50, 50, 50, 50, 200, 200, 200, 400, 400, 500, 500, 500};
void delay_us(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=10;  //????
      while(i--) ;    
   }
}
void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //????
      while(i--) ;    
   }
}
void Sound(u16 frq)
{
	u32 time;
	if(frq != 1000)
	{
		time = 500000/((u32)frq);
		GPIO_SetBits ( GPIOB, GPIO_Pin_12 );
		delay_us(time);
		GPIO_ResetBits ( GPIOB, GPIO_Pin_12 );
		delay_us(time);
	}else
		delay_us(1000);
}
void Sound2(u16 time)
{
    GPIO_SetBits ( GPIOB, GPIO_Pin_12 );
    delay_ms(time);
    GPIO_ResetBits ( GPIOB, GPIO_Pin_12 );
    delay_ms(time);
}
void play_successful(void)
{
    int id=0;
    for(id = 0 ;id < 12 ;id++)
    {
        Sound2(melody[id]);
    }
}
void play_failed(void)
{
    int id=0;
    for(id = 11 ;id >=0 ;id--)
    {
        Sound2(melody[id]);
    }
}
void play_music(void)
{
	//              ?7  1   2   3   4   5   6   7  ?1 ?2 ?3 ?4 ?5 ???
	uc16 tone[] = {247,262,294,330,349,392,440,294,523,587,659,698,784,1000};    
u8 music[]={5,5,6,8,7,6,5,6,13,13,5,5,6,8,7,6,5,3,13,13,2,2,3,5,3,5,6,3,2,1,6,6,5,6,5,3,6,5,13,13,
                  5,5,6,8,7,6,5,6,13,13,5,5,6,8,7,6,5,3,13,13,2,2,3,5,3,5,6,3,2,1,6,6,5,6,5,3,6,1,    
              13,8,9,10,10,9,8,10,9,8,6,13,6,8,9,9,8,6,9,8,6,5,13,2,3,5,5,3,5,5,6,8,7,6,6,10,9,9,8,6,5,6,8  };            
u8 time[] = {2,4,2,2,2,2,2,8,4, 4,2,4,2,2,2,2,2,8,4, 4, 2,4,2,4,2,2,4,2,2,8,2,4,2,2,2,2,2,8,4 ,4, 
                2,4,2,2,2,2,2,8,4, 4,2,4,2,2,2,2,2,8,4,4,2,4,2,4,2,2,4,2,2,8,2,4,2,2,2,2,2,8,
                4,2,2,2,4,2,2,2,2,2,8,4,2,2,2,4,2,2,2,2,2,8,4,2,2,2,4,2,2,5,2,6,2,4,2,2,2,4,2,4,2,2,12  };
	u32 yanshi;
	u16 i,e;
	yanshi = 10;
	for(i=0;i<sizeof(music)/sizeof(music[0]);i++){
		for(e=0;e<((u16)time[i])*tone[music[i]]/yanshi;e++){
			Sound((u32)tone[music[i]]);
		}	
	}
}
int main()
{
	uint32_t  keyvalue = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //GPIO_Pin_8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHZ
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	BEEP_Init();
	
	while(1) {
		play_music();
	}
}
