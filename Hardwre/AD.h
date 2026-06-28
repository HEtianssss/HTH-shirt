#ifndef __AD_H
#define __AD_H

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
int Get_Adc_Average(int ch,int times);
#endif
