#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启APB1外设的时钟,接入TIM2时基单元时钟模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);//初始化时基单元,TIM2的时基单元就由内部时钟来驱动
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up ;//上升分频计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;// x 10000  ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;//预分频   PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器 高级定时器的一种,这里用不到,直接给零;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCinitStructure;
	TIM_OCStructInit(&TIM_OCinitStructure);//给初始值的,就不用给值,不然就全部给值
	TIM_OCinitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //输出比较模式
	TIM_OCinitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出比较的极性 高
	TIM_OCinitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCinitStructure.TIM_Pulse = 0;   //设置CCR(捕获比较寄存器)
	TIM_OC2Init(TIM2,&TIM_OCinitStructure);
	TIM_OC3Init(TIM2,&TIM_OCinitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare2(uint8_t Compare)//单独更改通道2的ccr值
{
	TIM_SetCompare2(TIM2, Compare);
}

void PWM_SetCompare3(uint8_t Compare)//单独更改通道3的ccr值
{
	TIM_SetCompare3(TIM2, Compare);
}
