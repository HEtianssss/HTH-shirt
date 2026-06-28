#include "stm32f10x.h"                  // Device header

uint16_t jishu;
void duishesi_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式   默认高电平
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);//外设源  外设引脚定义
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断线的模式
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;//中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线的新状态
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组
	NVIC_InitTypeDef NVIC_InitStructure ;//定义结构体
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;//定义接口
	NVIC_InitStructure.NVIC_IRQChannelCmd =  ENABLE ;//使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);	
}


uint16_t jishufanhui_Get(void)
{
	return jishu;
}

void EXTI15_10_IRQHandler(void)//自动执行,不用调用函数;
{
	if(EXTI_GetITStatus(EXTI_Line14)==SET)//判断是否为1   SET=1;RESET=0
	{
		jishu ++ ;
		EXTI_ClearITPendingBit(EXTI_Line14);//运行完成,清除标志位
	}
}
