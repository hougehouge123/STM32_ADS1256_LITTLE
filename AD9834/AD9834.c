#include "AD9834.h"
/****************************************************************
函数名称: delay
功    能: 延时一下
参    数: 延的长短
返回值  : 无
*****************************************************************/
void AD9834_Delay(uint32_t time) {
	while(time--);
}
/****************************************************************
函数名称: AD9834_Write_16Bits
功    能: 向AD9834写入16为数据
参    数: data --  要写入的16位数据
返回值  : 无
*****************************************************************/
void AD9834_Write_16Bits(uint16_t data)
{
	  SCK_H;
      AD9834_FSYNC_CLR;
	  AD9834_Delay(1);
    for(int i=0; i<16; i++)
    {
			if(data & 0x8000)
			SDA_H;
			else
			SDA_L;
			AD9834_Delay(1);
			SCK_L;
			AD9834_Delay(1);
	    SCK_H;
		  data<<=1;
		}
		AD9834_Delay(1);
		AD9834_FSYNC_SET ;

}

/*
*********************************************************************************************************
*	函 数 名: AD9833_WaveSeting
*	功能说明: 向SPI总线发送16个bit数据
*	形    参:
				1.Freq: 频率值, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 或 1
			  3.WaveMode: 0(三角波),1(正弦波),2(方波)
			  4.Phase : 波形的初相位
*	返 回 值: 无
*********************************************************************************************************
*/ 
void AD9834_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;
    	uint16_t FS;
		/*********************************计算频率的16进制值***********************************/
		frequence_mid=268435456/75;//适合75M晶振
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //这个frequence_hex的值是32位的一个很大的数字，需要拆分成两个14位进行处理；
		frequence_LSB=frequence_hex; //frequence_hex低16位送给frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位
		frequence_MSB=frequence_hex>>14; //frequence_hex高16位送给frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//去除最高两位，16位数换去掉高位后变成了14位

		Phs_data=Phase|0xC000;	//相位值
		AD9834_Write_16Bits(0x0100); //复位AD9833,即RESET位为1
		AD9834_Write_16Bits(0x2100); //选择数据一次写入，B28位和RESET位为1

		if(Freq_SFR==0)				  //把数据设置到设置频率寄存器0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //使用频率寄存器0输出波形
			AD9834_Write_16Bits(frequence_LSB); //L14，选择频率寄存器0的低14位数据输入
			AD9834_Write_16Bits(frequence_MSB); //H14 频率寄存器的高14位数据输入
			AD9834_Write_16Bits(Phs_data);	//设置相位
			/**设置FSELECT位为0，芯片进入工作状态,频率寄存器0输出波形**/
	    FS=0X2000;
		}
		if(Freq_SFR==1)				//把数据设置到设置频率寄存器1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//使用频率寄存器1输出波形
			AD9834_Write_16Bits(frequence_LSB); //L14，选择频率寄存器1的低14位输入
			AD9834_Write_16Bits(frequence_MSB); //H14 频率寄存器1为
			AD9834_Write_16Bits(Phs_data);	//设置相位
		  /**设置FSELECT位为0，设置FSELECT位为1，即使用频率寄存器1的值，芯片进入工作状态,频率寄存器1输出波形**/
		  FS=0X2800;
		}

		if(WaveMode==0) //输出三角波波形
		 	AD9834_Write_16Bits(0x2002|FS); 
		if(WaveMode==1)	//输出正弦波形
			AD9834_Write_16Bits(0x2000|FS); 
		if(WaveMode==2)	//输出方波波形
			AD9834_Write_16Bits(0x2028|FS); 

}


