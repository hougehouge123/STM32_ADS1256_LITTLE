#include "AD9834.h"
/****************************************************************
��������: delay
��    ��: ��ʱһ��
��    ��: �ӵĳ���
����ֵ  : ��
*****************************************************************/
void AD9834_Delay(uint32_t time) {
	while(time--);
}
/****************************************************************
��������: AD9834_Write_16Bits
��    ��: ��AD9834д��16Ϊ����
��    ��: data --  Ҫд���16λ����
����ֵ  : ��
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
*	�� �� ��: AD9833_WaveSeting
*	����˵��: ��SPI���߷���16��bit����
*	��    ��:
				1.Freq: Ƶ��ֵ, 0.1 hz - 12Mhz
			  2.Freq_SFR: 0 �� 1
			  3.WaveMode: 0(���ǲ�),1(���Ҳ�),2(����)
			  4.Phase : ���εĳ���λ
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void AD9834_WaveSeting(double Freq,unsigned int Freq_SFR,unsigned int WaveMode,unsigned int Phase )
{

		int frequence_LSB,frequence_MSB,Phs_data;
		double   frequence_mid,frequence_DATA;
		long int frequence_hex;
    	uint16_t FS;
		/*********************************����Ƶ�ʵ�16����ֵ***********************************/
		frequence_mid=268435456/75;//�ʺ�75M����
		frequence_DATA=Freq;
		frequence_DATA=frequence_DATA/1000000;
		frequence_DATA=frequence_DATA*frequence_mid;
		frequence_hex=frequence_DATA;  //���frequence_hex��ֵ��32λ��һ���ܴ�����֣���Ҫ��ֳ�����14λ���д���
		frequence_LSB=frequence_hex; //frequence_hex��16λ�͸�frequence_LSB
		frequence_LSB=frequence_LSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ
		frequence_MSB=frequence_hex>>14; //frequence_hex��16λ�͸�frequence_HSB
		frequence_MSB=frequence_MSB&0x3fff;//ȥ�������λ��16λ����ȥ����λ������14λ

		Phs_data=Phase|0xC000;	//��λֵ
		AD9834_Write_16Bits(0x0100); //��λAD9833,��RESETλΪ1
		AD9834_Write_16Bits(0x2100); //ѡ������һ��д�룬B28λ��RESETλΪ1

		if(Freq_SFR==0)				  //���������õ�����Ƶ�ʼĴ���0
		{
		 	frequence_LSB=frequence_LSB|0x4000;
		 	frequence_MSB=frequence_MSB|0x4000;
			 //ʹ��Ƶ�ʼĴ���0�������
			AD9834_Write_16Bits(frequence_LSB); //L14��ѡ��Ƶ�ʼĴ���0�ĵ�14λ��������
			AD9834_Write_16Bits(frequence_MSB); //H14 Ƶ�ʼĴ����ĸ�14λ��������
			AD9834_Write_16Bits(Phs_data);	//������λ
			/**����FSELECTλΪ0��оƬ���빤��״̬,Ƶ�ʼĴ���0�������**/
	    FS=0X2000;
		}
		if(Freq_SFR==1)				//���������õ�����Ƶ�ʼĴ���1
		{
			 frequence_LSB=frequence_LSB|0x8000;
			 frequence_MSB=frequence_MSB|0x8000;
			//ʹ��Ƶ�ʼĴ���1�������
			AD9834_Write_16Bits(frequence_LSB); //L14��ѡ��Ƶ�ʼĴ���1�ĵ�14λ����
			AD9834_Write_16Bits(frequence_MSB); //H14 Ƶ�ʼĴ���1Ϊ
			AD9834_Write_16Bits(Phs_data);	//������λ
		  /**����FSELECTλΪ0������FSELECTλΪ1����ʹ��Ƶ�ʼĴ���1��ֵ��оƬ���빤��״̬,Ƶ�ʼĴ���1�������**/
		  FS=0X2800;
		}

		if(WaveMode==0) //������ǲ�����
		 	AD9834_Write_16Bits(0x2002|FS); 
		if(WaveMode==1)	//������Ҳ���
			AD9834_Write_16Bits(0x2000|FS); 
		if(WaveMode==2)	//�����������
			AD9834_Write_16Bits(0x2028|FS); 

}


