//极简版DAC8830驱动
//2022.7.4
//@HZZ
#ifndef __DAC8830_H
#define __DAC8830_H

#include "main.h"

/* 设置参考电压，单位mv*/
#define VREF 2500

/* DAC8830 控制引脚 */
//片选引脚
#define CS_H   HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_3,1)
#define CS_L  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_3,0)
//模拟SPI时钟引脚
#define SCK_H  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_4,1)
#define SCK_L  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_4,0)
//模拟SPI数据输出引脚
#define SDA_H  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_5,1)
#define SDA_L  HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_5,0)

/* AD9834函数声明 */
void DAC8830_Write_16Bits(uint16_t data) ;
void DAC8830_Set_Output(uint16_t Voltage_mv) ;
#endif /* DAC8830_H */
