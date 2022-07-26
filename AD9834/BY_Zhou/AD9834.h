#ifndef DDS_AD9834_H
#define DDS_AD9834_H

#ifdef __cplusplus
extern "C" {
#endif

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
#define AD9834_MASK 0xFFFF


#define AD9834_SW_ENABLE_MASK 0xFDFF
#define AD9834_RESET_ENABLE (1<<8)

#define AD9834_FS_0_MASK 0xF7FF
#define AD9834_FS_1 (1<<11)

#define AD9834_PS_0_MASK 0xFBFF
#define AD9834_PS_1 (1<<10)

#define AD9834_SLEEP1_DISABLE_MASK 0xFF7F
#define AD9834_SLEEP1_ENABLE (1<<7)

#define AD9834_FREQ_0_REGISTER_PREFIX 0x4000
#define AD9834_FREQ_1_REGISTER_PREFIX 0x8000
#define AD9834_FREQ_LSB_MASK 0x3FFF
#define AD9834_FREQ_MSB_OFFSET(x) (x>>14)
#define AD9834_FREQ_RESERVED_VALUE (1<<30)
#define AD9834_B28_ENABLE (1<<13)
#define AD9834_B28_MASK 0xDFFF
#define AD9834_HLB_ENABLE (1<<12)
#define AD9834_HLB_MASK 0xEFFF
#define AD9834_INTERNAL_FREQ (75*1000*1000)
#define AD9834_FREQ_FACTOR (1<<28)

#define AD9834_PHASE_0_REGISTER_PREFIX 0xC000
#define AD9834_PHASE_1_REGISTER_PREFIX 0xE000
#define PI 3.14159265358979f
#define AD9834_PHASE_FACTOR (1<<12)

#define AD9834_OPBITEN_MASK 0xFFDF
#define AD9834_OPBITEN_ENABLE (1<<5)
#define AD9834_MODE_MASK 0xFFFD
#define AD9834_MODE_ENABLE (1<<1)
#define AD9834_SIGN_PIB_ENABLE (1<<4)
#define AD9834_SIGN_PIB_MASK 0xFFEF
#define AD9834_DIV_2_ENABLE (1<<3)
#define AD9834_DIV_2_MASK 0xFFF7



#define AD9834_PIN_ENABLE (1<<9)
#define AD9834_SW_ENABLE 0

#define AD9834_FREQ_0 0
#define AD9834_FREQ_1 1

#define AD9834_PHASE_0 0
#define AD9834_PHASE_1 1

#define AD9834_MODE_SINE_WAVE 0
#define AD9834_MODE_TRI_WAVE 1
#define AD9834_MODE_SQUARE_WAVE 2

#define AD9834_SIGN_MODE_DAC_MSB_2 0
#define AD9834_SIGN_MODE_DAC_MSB 1
#define AD9834_SIGN_MODE_COMPARE_OUT 2

#include "main.h"
void initAD9834(GPIO_TypeDef * _SCLK,uint16_t _SCLK_PIN,GPIO_TypeDef * _SDA,uint16_t _SDA_PIN,GPIO_TypeDef * _FSYNC,uint16_t _FSYNC_PIN,GPIO_TypeDef * _FS,uint16_t _FS_PIN,GPIO_TypeDef * _PS,uint16_t _PS_PIN,GPIO_TypeDef * _RST,uint16_t _RST_PIN,GPIO_TypeDef * _SLEEP,uint16_t _SLEEP_PIN,uint16_t pin_sw_bit);
void AD9834_setSignOutput(uint8_t sign_mode);
void AD9834_setOutput(uint8_t mode);
void AD9834_setPhase(uint8_t target,float phase);
void AD9834_setFreqMSBExternal(uint8_t target,float freq,float base);
void AD9834_setFreqLSBExternal(uint8_t target,float freq,float base);
void AD9834_setFreqMSB(uint8_t target,float freq);
void AD9834_setFreqLSB(uint8_t target,float freq);
void AD9834_setFreqExternal2(uint8_t target,float freq,float base);
void AD9834_setFreq2(uint8_t target,float freq);
void AD9834_setFreqExternal(float freq0,float freq1,float base);
void AD9834_setFreq(float freq0,float freq1);
void AD9834_setPinFlag(uint16_t pin_sw_bit);
void AD9834_setOutputChannel(uint8_t freq,uint8_t phase);

#ifdef __cplusplus
}
#endif

#endif //DDS_AD9834_H
