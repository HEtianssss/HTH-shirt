#ifndef __DH11_H
#define __DH11_H

void DHT_Init(GPIOMode_TypeDef gpio_mode);
uint8_t DHT_HUM_TEM(uint8_t buffer[]);
#endif

