#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启APB1外设的时钟,接入TIM2时基单元时钟模式
	
	TIM_InternalClockConfig(TIM2);//时基单元,TIM2的时基单元就由内部时钟来驱动
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up ;//上升分频计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;// x 10000
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;//预分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器 高级定时器的一种,这里用不到,直接给零;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//手动清楚标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能中段,表明计时器到一定的数开始中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitSructure;
	NVIC_InitSructure.NVIC_IRQChannel = TIM2_IRQn;//TIM2在NIVIC的通道
	NVIC_InitSructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitSructure.NVIC_IRQChannelPreemptionPriority =  2;
	NVIC_InitSructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitSructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler()//定时器的中断函数,自动执行
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)//检查中断标志位
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
	}
}


