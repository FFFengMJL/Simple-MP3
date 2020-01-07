#include "oled.h"
#include "stdlib.h"
#include "delay.h"
#include "oledfont.h"
#include "myiic.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 	


u8 OLED_GRAM[128][8];	 
void WriteCmd(u8 command)
{
    IIC_Start();
    IIC_Send_Byte(0x78);//OLED地址
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);//寄存器地址
    IIC_Wait_Ack();
    IIC_Send_Byte(command);
    IIC_Wait_Ack();
    IIC_Stop();
}


void WriteDat(u8 data)
{
    IIC_Start();
    IIC_Send_Byte(0x78);//OLED地址
    IIC_Wait_Ack();
    IIC_Send_Byte(0x40);//寄存器地址
    IIC_Wait_Ack();
    IIC_Send_Byte(data);
    IIC_Wait_Ack();
    IIC_Stop();
}



//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WriteCmd (0xb0+i);    //设置页地址（0~7）
		WriteCmd (0x00);      //设置显示位置—列低地址
		WriteCmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			WriteDat(OLED_GRAM[n][i]); 
	}   
}
  
//开启OLED显示    
void OLED_Display_On(void)
{
	WriteCmd(0X8D);  //SET DCDC命令
	WriteCmd(0X14);  //DCDC ON
	WriteCmd(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	WriteCmd(0X8D);  //SET DCDC命令
	WriteCmd(0X10);  //DCDC OFF
	WriteCmd(0XAE);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[127-x][pos]|=temp;
	else OLED_GRAM[127-x][pos]&=~temp;	    
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint_o(u8 x,u8 y, u8 t)
{
	u8 i,m,n;
	if (x > 127 || y > 63) return;
	i=y/8;
	m=y%8;
	n=1<<m;
	if (t == 1)OLED_GRAM[x][i]|=n;
	else OLED_GRAM[x][i]&=n;
}


//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y, dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}



//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
			if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
			else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
			else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
			else return;								//没有的字库
			for(t1=0;t1<8;t1++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,mode);
				else OLED_DrawPoint(x,y,!mode);
				temp<<=1;
				y++;
				if((y-y0)==size)
				{
					y=y0;
					x++;
					break;
				}
			}  	 
    }          
}
		  

//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	

//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}		

//??2???
//x,y :????	 
//len :?????
//size:????
//mode:??	0,????;1,????
//num:??(0~4294967295);	 	  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
void fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void picture_1(void)
{
  unsigned char x,y;
  unsigned int i=0;
  for(y=0;y<8;y++)
    {
      WriteCmd(0xb0+y);
      WriteCmd(0x00);
      WriteCmd(0x10);
      for(x=0;x<128;x++)
        {
          WriteDat(testStr[i++]);
        }
    }
}

void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=name[chr1][i];}//调用16*16字体
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y,1);
								else OLED_DrawPoint(x,y,0);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}


void picture_2(void)
{
  unsigned char x,y;
  unsigned int i=0;
  for(y=0;y<2;y++)
    {
      WriteCmd(0xb0+y);
      WriteCmd(0x00);
      WriteCmd(0x10);
      for(x=0;x<128;x++)
        {
          WriteDat(lyj[i++]);
        }
    }
}

void OLED_Test(void)
{
	fill_picture(0xff);
	delay_ms(500);
	fill_picture(0x00);
	delay_ms(500);
}
//初始化SSD1306					    
void OLED_Init(void)
{
	delay_ms(100);
	WriteCmd(0xAE); //关闭显示	
	WriteCmd(0xD5); //设置时钟分频因子,震荡频率
	WriteCmd(80);   //[3:0],分频因子;[7:4],震荡频率
	WriteCmd(0xA8); //设置驱动路数
	WriteCmd(0X3F); //默认0X3F(1/64) 
	WriteCmd(0xD3); //设置显示偏移
	WriteCmd(0X00); //默认为0
	WriteCmd(0x40); //设置显示开始行 [5:0],行数.										    
	WriteCmd(0x8D); //电荷泵设置
	WriteCmd(0x14); //bit2，开启/关闭
	WriteCmd(0x20); //设置内存地址模式
	WriteCmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	WriteCmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	WriteCmd(0xC8); //!!!!设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	WriteCmd(0xDA); //设置COM硬件引脚配置
	WriteCmd(0x12); //[5:4]配置	 
	WriteCmd(0x81); //对比度设置
	WriteCmd(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
	WriteCmd(0xD9); //设置预充电周期
	WriteCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	WriteCmd(0xDB); //设置VCOMH 电压倍率
	WriteCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	WriteCmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	WriteCmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	WriteCmd(0xAF); //开启显示	 
	OLED_Clear();
}  
