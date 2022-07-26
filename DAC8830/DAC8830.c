#include "DAC8830.h"

/****************************************************************
函数名称: delay
功    能: 延时一下
参    数: 延的长短
返回值  : 无
*****************************************************************/
void delay(uint32_t time) {
	while(time--);
}

/****************************************************************
函数名称: DAC8830_Write_16Bits
功    能: 向DAC8830写入16为数据
参    数: data --  要写入的16位数据
返回值  : 无
*****************************************************************/
void DAC8830_Write_16Bits(uint16_t data)
{
	SCK_L;
	CS_L;
	delay(15);
	for(int i=0; i<16; i++)
	{
		if(data & 0x8000) SDA_H;
		else SDA_L;
		delay(15);
	  SCK_L;
		delay(15);
		SCK_H;
		data<<= 1;
	}
	delay(15);
	CS_H;
}

/****************************************************************
函数名称: DAC8830_Set_Output
功    能: 向DAC8830设置输出电压 
参    数: Voltage_mv，单位为mv ，范围为0-VREF 
返回值  : 无
*****************************************************************/
void DAC8830_Set_Output(uint16_t Voltage_mv)
{
	uint16_t Vol_Value=0;
	Vol_Value = (uint16_t) ((float)Voltage_mv/(float)VREF*(float)65535);
	DAC8830_Write_16Bits(Vol_Value);
}
