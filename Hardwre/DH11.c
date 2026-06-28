#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "Delay.h"

char str[50];
char DHT_Buffer[5];

void DHT_Init(GPIOMode_TypeDef gpio_mode)  
{
	GPIO_InitTypeDef GPIO_InitStr;	// 定义结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//开启GPIO时钟给C口,因为用到P13 C
	
	GPIO_InitStr.GPIO_Mode = gpio_mode;   //手动输入模式
	GPIO_InitStr.GPIO_Pin= GPIO_Pin_13;  
	GPIO_InitStr.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStr); //初始化结构体
	
	GPIO_SetBits(GPIOA,GPIO_Pin_13);    //给高电平到P13 C
}

uint8_t DHT_Start()    //发送起始信号
{
	DHT_Init(GPIO_Mode_Out_PP);		//配置为输出状态
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);  //拉低引脚(低电平)
	Delay_ms(20);   //等待  (主机起始信号拉低时间)
	GPIO_SetBits(GPIOC,GPIO_Pin_13);   //拉高引脚(高电平)
	Delay_us(13);   //等待(主机释放总线时间)
	
	DHT_Init (GPIO_Mode_IPU); //配置为输入状态
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))  //判断模块是否向单片机发送应答信号(引脚拉低)
	{
		while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));//等待拉低结束
		while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));//如果拉低结束就发送高电平信号
		return 1;  //如果接收到应答信号,返回状态
	}
	return 0;
}

uint8_t DHT_Get_Date()  //收到40位数据
{ 
	uint8_t buff = 0;    //定义8位变量  存数据
	for(int i = 0;i<8;i++)
	{
		buff <<= 1;  //左移  腾空间
		while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));//拉低电平
		Delay_us(30);  //等待
		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) ? (buff |= 0x01):(buff &=~0x01); //判断引脚状态  如果高电平给位写上1,否则低为0
		while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));//等待高电平结束
	}
	return buff;  //返回获取8位的数据
}

uint8_t DHT_HUM_TEM(uint8_t buffer[])
{
	if(DHT_Start())
	{
		buffer[0] = DHT_Get_Date(); //每8位存储到一个数组  湿度整数
		buffer[1] = DHT_Get_Date(); //每8位存储到一个数组  湿度小数
		buffer[2] = DHT_Get_Date(); //每8位存储到一个数组  温度整数
		buffer[3] = DHT_Get_Date(); //每8位存储到一个数组  温度小数
		buffer[4] = DHT_Get_Date(); //每8位存储到一个数组  校验和
		Delay_us(60);    //结束
	}
	if(buffer[4] == (buffer[0]+buffer[1]+buffer[2]+buffer[3]))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}













