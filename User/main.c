#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "OLED.H"
#include "delay.h"
#include "Key.h"
#include "Buzzer.h"
#include "stdio.h"
#include "DH11.H"
#include "AD.H"
/*
                 ----------------------------------------------        
			 	      	|                         |
		 			    |						  |
					    |                         |
					    |                         |
PWM_SetCompare2		    |                         |               PWM_SetCompare3
					    |                         |
			 		    |                         |
后端2500	       左一	|                         |  右一       后端  500
中间1500          |||||||||||                   |||||||||||		中间 1500							
前端500		      |||||||||||                   |||||||||||     前端2500
			      |||||||||||				    |||||||||||
				  
*/
extern char str;  // 因为已在DH11模块定义,添加extern连接定义  要删除数值
extern uint8_t DHT_Buffer;// 因为已在DH11模块定义,添加extern连接定义   要删除数值
uint16_t AD0,AD5;  //定义雨滴的数值范围(AD0:PA0)   风速数值范围(AD5:PA5)  
float Voltage;     //小数型
int main()
{
	int i,j,a=0,b=0; 
	float wind,twind;     //风速数据	
	char str[50];         //湿度的数组 温度8位  温度的小数8位  湿度8位  湿度的小数8位  校验和位8位  
	uint8_t KeyNum;       //按钮的返回值  
	uint8_t DHT_Buffer[5]; //湿度模组的数值
	volatile uint16_t desiredValue = 1500; //电压初始值
	volatile uint16_t currentValue = 0;   //转换后的初始值
	
	AD_Init();   //数模转换初始化
	PWM_Init (); //pwm初始化
	Key_Init();  //按钮初始化
	OLED_Init();  //显示屏初始化
	Buzzer_Init();  //蜂鸣器初始化
	void DHT_Init();  //温湿度初始化
	
	PWM_SetCompare2(1500);  //将舵机回正 单独设置ccr
	PWM_SetCompare3(1500);  //将舵机回正 单独设置ccr
		
	while(1)
	{		
		KeyNum = Key_GetNum ();  //返回确认按键
		
		AD0 = AD_GetValue(ADC_Channel_0); //雨滴返回数值
		Voltage = (float)AD0 / 4095 * 3.3; //雨滴计算公式
		
		AD5=AD_GetValue(ADC_Channel_5);//风速AD值
		wind = (float)(Get_Adc_Average(5,10))*(3.3/4096);//计算公式
		twind = 300*wind;
		
				OLED_ShowString(3, 1, "Volatge:0.00V");  //雨滴转化后
				OLED_ShowNum(3, 9, Voltage, 1);
				OLED_ShowNum(3, 11, (uint16_t)(Voltage * 100) % 100, 2);
				
				OLED_ShowString(4, 1, "twind:V");  //风速转化后
				OLED_ShowNum(4, 7, twind, 3);
		
		if(KeyNum == 1) //按钮1
		{
			Buzzer_ON();  //蜂鸣器开
			Delay_ms(100);  //延迟
			Buzzer_OFF(); //蜂鸣器关
			for(i=0;i<=1000;i++)//调角度
			{
			    OLED_ShowChinese(1,7,0);  //伸
				OLED_ShowChinese(1,8,1);  //出
				PWM_SetCompare2(1500-i);    //从中间往前
				PWM_SetCompare3(1500+i); 
				Delay_ms(6);//调时间
			}
			
		}
		if(KeyNum == 2)
		{
			Buzzer_ON();
			Delay_ms(100);
			Buzzer_OFF();
			
			for(i=0;i<=1000;i++)//调角度
			{
				 OLED_ShowChinese(1,7,2);  //收
				OLED_ShowChinese(1,8,3);   //放
				PWM_SetCompare2(500+i);  //从前往中
				PWM_SetCompare3(2500-i);
				Delay_ms(6);//调时间
			}
		}
		
		if(KeyNum == 3)
		{
			Buzzer_ON();
			Delay_ms(100);
			Buzzer_OFF();
			
			for(j=0;j<=1000;j++)//调角度
			{
				 OLED_ShowChinese(1,7,4);  //回
				 OLED_ShowChinese(1,8,5);  //避
				 PWM_SetCompare2(1500+j);  //从中往后
				 PWM_SetCompare3(1500-j);
				 Delay_ms(6);//调时间
			}
		}
		
		if(KeyNum == 4)
		{
			Buzzer_ON();
			Delay_ms(100);
			Buzzer_OFF();
			
			for(j=0;j<=1000;j++)//调角度
			{
				 OLED_ShowChinese(1,7,2);  //收
				 OLED_ShowChinese(1,8,3);  //放
				 PWM_SetCompare2(2500-j);   //从后往中
				 PWM_SetCompare3(500+j);
				 Delay_ms(6);//调时间
			}
		}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////		
		if(Voltage<1.4)//雨滴
			{
				if(b==0)
				{
					for(j=0;j<=2000;j++)//调角度
					{
					 OLED_ShowChinese(1,7,4);  //伸
					 OLED_ShowChinese(1,8,5);  //出
					 PWM_SetCompare2(500+j);    
					 PWM_SetCompare3(2500-j);
					 Delay_ms(6);//调时间
					}
				}
				b++;
				if(Voltage>=1.4)
				{
					b--;
				}
			}
//////////////////////////////////////////////////////////////			
			if(DHT_Buffer[0]>90) //温湿度
			{
				if(a==0)
				{
					for(j=0;j<=2000;j++)//调角度
					{
					 OLED_ShowChinese(1,7,4);  //伸
					 OLED_ShowChinese(1,8,5);  //出
					 PWM_SetCompare2(500+j);    
					 PWM_SetCompare3(2500-j);
					 Delay_ms(6);//调时间
					}
				}
				a++;
				if(DHT_Buffer[0]<90)
				{
					a--;
				}
			}
//////////////////////////////////////////////////////////////	
			if(twind>35)//风速
			{
					for(j=0;j<=2000;j++)//调角度
					{
					 OLED_ShowChinese(1,7,4);  //伸
					 OLED_ShowChinese(1,8,5);  //出
					 PWM_SetCompare2(500+j);    
					 PWM_SetCompare3(2500-j);
					 Delay_ms(6);//调时间
					}
			}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////				
			
			if(DHT_HUM_TEM(DHT_Buffer))  //显示字
			{
				sprintf(str,"Temp:%d.%d",DHT_Buffer[2],DHT_Buffer[3]);//温度
				OLED_ShowString(1,1,str);
				sprintf(str,"Humi:%d.%d",DHT_Buffer[0],DHT_Buffer[1]);//湿度
				OLED_ShowString(2,1,str);			
			}
				Delay_ms(2000);
		}
}

