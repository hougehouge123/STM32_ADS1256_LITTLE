//�����ADS9834����
//ħ��2022.7.1
//@HZZ
#ifndef __AD9834_H
#define __AD9834_H
#include "main.h"

/* AD9834 �������� */
//Ƭѡ����
#define AD9834_FSYNC_SET   HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_3,1)
#define AD9834_FSYNC_CLR   HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_3,0)
//ģ��SPIʱ������
#define SCK_H  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_4,1)
#define SCK_L  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_4,0)
//ģ��SPI��������
#define SDA_H  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_5,1)
#define SDA_L  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_5,0)

/* AD9834�������� */
void AD9834_Write_16Bits(uint16_t data) ;
void AD9834_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase );
#endif /* AD9834_H */
