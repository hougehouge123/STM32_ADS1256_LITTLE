#include "ad9834.h"

GPIO_TypeDef * SCLK;
uint16_t SCLK_PIN;
GPIO_TypeDef * SDA;
uint16_t SDA_PIN;
GPIO_TypeDef * FSYNC;
uint16_t FSYNC_PIN;
GPIO_TypeDef * FS;
uint16_t FS_PIN;
GPIO_TypeDef * PS;
uint16_t PS_PIN;
GPIO_TypeDef * RST;
uint16_t RST_PIN;
GPIO_TypeDef * SLEEP;
uint16_t SLEEP_PIN;

uint8_t pinFlag = 2;
// make sure when init hardware RESET is 1



uint32_t frequency0 = AD9834_FREQ_RESERVED_VALUE;
uint32_t frequency1 = AD9834_FREQ_RESERVED_VALUE;
uint32_t baseFrequency = AD9834_INTERNAL_FREQ;
//the three values are not used now



uint16_t controlRegisterValue = 0;

static void addAndSet(uint16_t offset);
static void removeAndSet(uint16_t mask);
static void addAndTempAndSet(uint16_t offset,uint16_t temp);
static void addAndRemoveAndTempAndSet(uint16_t add,uint16_t remove,uint16_t temp);
static void set();
static void addAndRemoveAndSet(uint16_t offset,uint16_t mask);
static void tempAndSet(uint16_t temp);


static void delay(uint32_t time) {
	while(time--);
}

static void AD9834_SCLK(uint8_t status) {
	HAL_GPIO_WritePin(SCLK,SCLK_PIN,status);
}

void AD9834_SCLK_HIGH() {
	AD9834_SCLK(HIGH);
}

void AD9834_SCLK_LOW() {
	AD9834_SCLK(LOW);
}

static void AD9834_SDA(uint8_t status) {
	HAL_GPIO_WritePin(SDA,SDA_PIN,status);
}

void AD9834_SDA_HIGH() {
	AD9834_SDA(HIGH);
}

void AD9834_SDA_LOW() {
	AD9834_SDA(LOW);
}

static void AD9834_FSYNC(uint8_t status) {
	HAL_GPIO_WritePin(FSYNC,FSYNC_PIN,status);
}

void AD9834_FSYNC_HIGH() {
	AD9834_FSYNC(HIGH);
}

void AD9834_FSYNC_LOW() {
	AD9834_FSYNC(LOW);
}

static void AD9834_FS(uint8_t status) {
	if (pinFlag)
		HAL_GPIO_WritePin(FS,FS_PIN,status);
	else if (status)
		addAndSet(AD9834_FS_1);
	else removeAndSet(AD9834_FS_0_MASK);
}

void AD9834_FS_HIGH() {
	AD9834_FS(HIGH);
}

void AD9834_FS_LOW() {
	AD9834_FS(LOW);
}

static void AD9834_PS(uint8_t status) {
	if (pinFlag)
		HAL_GPIO_WritePin(PS,PS_PIN,status);
	else if (status)
		addAndSet(AD9834_PS_1);
	else removeAndSet(AD9834_PS_0_MASK);
}

void AD9834_PS_HIGH() {
	AD9834_PS(HIGH);
}

void AD9834_PS_LOW() {
	AD9834_PS(LOW);
}

static void AD9834_RST(uint8_t status) {
	if (pinFlag)
		HAL_GPIO_WritePin(RST,RST_PIN,status);
	else if (status)
		addAndTempAndSet(0,AD9834_RESET_ENABLE);
}

void AD9834_RST_HIGH() {
	AD9834_RST(HIGH);
}

void AD9834_RST_LOW() {
	AD9834_RST(LOW);
}

static void AD9834_SLEEP(uint8_t status) {
	HAL_GPIO_WritePin(SLEEP,SLEEP_PIN,status);
}

void AD9834_SLEEP_HIGH() {
	AD9834_SLEEP(HIGH);
}

void AD9834_SLEEP_LOW() {
	AD9834_SLEEP(LOW);
}

void AD9834_writeData(uint16_t data) {
	AD9834_FSYNC_LOW();

	delay(1);

	for (int i = 0;i<16;i++) {
		if (data & 0x8000UL)
			AD9834_SDA_HIGH();
		else AD9834_SDA_LOW();
		delay(1);
		AD9834_SCLK_LOW();
		data<<=1;
		delay(1);
		AD9834_SCLK_HIGH();
	}

	delay(1);
	AD9834_FSYNC_HIGH();
}

static void addAndRemoveAndTempAndSet(uint16_t add,uint16_t remove,uint16_t temp) {
	controlRegisterValue |= add;
	controlRegisterValue &= remove;
	AD9834_writeData(controlRegisterValue | temp);
}

static void addAndTempAndSet(uint16_t offset,uint16_t temp) {
	addAndRemoveAndTempAndSet(offset,AD9834_MASK,temp);
}

static void addAndSet(uint16_t offset) {
	addAndTempAndSet(offset,0);
}

static void addAndRemoveAndSet(uint16_t offset,uint16_t mask) {
	addAndRemoveAndTempAndSet(offset,mask,0);
}

static void removeAndSet(uint16_t mask) {
	addAndRemoveAndSet(0,mask);
}

static void set() {
	addAndSet(0);
}

static void tempAndSet(uint16_t temp) {
	addAndTempAndSet(0,temp);
}

//pin_sw_bit: AD9834_PIN_ENABLE or AD9834_SW_ENABLE
void AD9834_setPinFlag(uint16_t pin_sw_bit) {
	if (pin_sw_bit)
		addAndSet(pin_sw_bit);
	else removeAndSet(AD9834_SW_ENABLE_MASK);
}

//spi mode 2
//pin_sw_bit: AD9834_PIN_ENABLE or AD9834_SW_ENABLE
void initAD9834(GPIO_TypeDef * _SCLK,uint16_t _SCLK_PIN,GPIO_TypeDef * _SDA,uint16_t _SDA_PIN,GPIO_TypeDef * _FSYNC,uint16_t _FSYNC_PIN,GPIO_TypeDef * _FS,uint16_t _FS_PIN,GPIO_TypeDef * _PS,uint16_t _PS_PIN,GPIO_TypeDef * _RST,uint16_t _RST_PIN,GPIO_TypeDef * _SLEEP,uint16_t _SLEEP_PIN,uint16_t pin_sw_bit) {
	SCLK = _SCLK;
	SCLK_PIN = _SCLK_PIN;
	SDA = _SDA;
	SDA_PIN = _SDA_PIN;
	FSYNC = _FSYNC;
	FSYNC_PIN = _FSYNC_PIN;
	FS = _FS;
	FS_PIN = _FS_PIN;
	PS = _PS;
	PS_PIN = _PS_PIN;
	RST = _RST;
	RST_PIN = _RST_PIN;
	SLEEP = _SLEEP;
	SLEEP_PIN = _SLEEP_PIN;
	AD9834_RST_HIGH();
	HAL_Delay(1);
	AD9834_RST_LOW();
	pinFlag = pin_sw_bit ? 1:0;
	AD9834_FSYNC_HIGH();
	AD9834_SCLK_HIGH();
	if (!pinFlag)
		tempAndSet(AD9834_RESET_ENABLE);
	else addAndSet(AD9834_PIN_ENABLE);
	HAL_Delay(1);
	if (!pinFlag)
		set();
}

//internal
void AD9834_setFreq(float freq0,float freq1) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	frequency0 = freq0 / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	uint16_t freq0_28_lsb = AD9834_FREQ_0_REGISTER_PREFIX | (frequency0 & AD9834_FREQ_LSB_MASK);
	uint16_t freq0_28_msb = AD9834_FREQ_0_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency0));
	frequency1 = freq1 / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	uint16_t freq1_28_lsb = AD9834_FREQ_1_REGISTER_PREFIX | (frequency1 & AD9834_FREQ_LSB_MASK);
	uint16_t freq1_28_msb = AD9834_FREQ_1_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency1));
	AD9834_writeData(freq0_28_lsb);
	AD9834_writeData(freq0_28_msb);
	AD9834_writeData(freq1_28_lsb);
	AD9834_writeData(freq1_28_msb);
}

void AD9834_setFreqExternal(float freq0,float freq1,float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	frequency0 = freq0 / base * AD9834_FREQ_FACTOR;
	uint16_t freq0_28_lsb = AD9834_FREQ_0_REGISTER_PREFIX | (frequency0 & AD9834_FREQ_LSB_MASK);
	uint16_t freq0_28_msb = AD9834_FREQ_0_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency0));
	frequency1 = freq1 / base * AD9834_FREQ_FACTOR;
	uint16_t freq1_28_lsb = AD9834_FREQ_1_REGISTER_PREFIX | (frequency1 & AD9834_FREQ_LSB_MASK);
	uint16_t freq1_28_msb = AD9834_FREQ_1_REGISTER_PREFIX | (AD9834_FREQ_MSB_OFFSET(frequency1));
	AD9834_writeData(freq0_28_lsb);
	AD9834_writeData(freq0_28_msb);
	AD9834_writeData(freq1_28_lsb);
	AD9834_writeData(freq1_28_msb);
}

// not recommended by datasheet
// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreq2(uint8_t target,float freq) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeData(freq_28_lsb);
	AD9834_writeData(freq_28_msb);
}

// not recommended by datasheet
// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqExternal2(uint8_t target,float freq,float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (!(controlRegisterValue & AD9834_B28_ENABLE))
		addAndSet(AD9834_B28_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / base * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeData(freq_28_lsb);
	AD9834_writeData(freq_28_msb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqLSB(uint8_t target,float freq) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (controlRegisterValue & AD9834_HLB_ENABLE)
		removeAndSet(AD9834_HLB_MASK);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	AD9834_writeData(freq_28_lsb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqMSB(uint8_t target,float freq) {
	baseFrequency = AD9834_INTERNAL_FREQ;
	if (controlRegisterValue & AD9834_SLEEP1_ENABLE)
		removeAndSet(AD9834_SLEEP1_DISABLE_MASK);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (!(controlRegisterValue & AD9834_HLB_ENABLE))
		addAndSet(AD9834_HLB_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / AD9834_INTERNAL_FREQ * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeData(freq_28_msb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqLSBExternal(uint8_t target,float freq,float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (controlRegisterValue & AD9834_HLB_ENABLE)
		removeAndSet(AD9834_HLB_MASK);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / base * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_lsb = prefix | (freq_28 & AD9834_FREQ_LSB_MASK);
	AD9834_writeData(freq_28_lsb);
}

// target: AD9834_FREQ_0 or AD9834_FREQ_1
void AD9834_setFreqMSBExternal(uint8_t target,float freq,float base) {
	baseFrequency = base;
	if (!(controlRegisterValue & AD9834_SLEEP1_ENABLE))
		addAndSet(AD9834_SLEEP1_ENABLE);
	if (controlRegisterValue & AD9834_B28_ENABLE)
		removeAndSet(AD9834_B28_MASK);
	if (!(controlRegisterValue & AD9834_HLB_ENABLE))
		addAndSet(AD9834_HLB_ENABLE);
	uint16_t prefix;
	if (target)
		prefix = AD9834_FREQ_1_REGISTER_PREFIX;
	else
		prefix = AD9834_FREQ_0_REGISTER_PREFIX;
	uint32_t freq_28 = freq / base * AD9834_FREQ_FACTOR;
	if (target)
		frequency1 = freq_28;
	else frequency0 = freq_28;
	uint16_t freq_28_msb = prefix | (AD9834_FREQ_MSB_OFFSET(freq_28));
	AD9834_writeData(freq_28_msb);
}

// target: AD9834_PHASE_0 or AD9834_PHASE_1
void AD9834_setPhase(uint8_t target,float phase) {
	uint16_t prefix;
	if (target)
		prefix = AD9834_PHASE_1_REGISTER_PREFIX;
	else
		prefix = AD9834_PHASE_0_REGISTER_PREFIX;
	uint16_t phase_12 = phase * AD9834_PHASE_FACTOR / 2 / PI;
	AD9834_writeData(prefix | phase_12);
}

// mode: AD9834_MODE_SINE_WAVE, AD9834_MODE_TRI_WAVE, AD9834_MODE_SQUARE_WAVE
void AD9834_setOutput(uint8_t mode) {
	if (!mode)
		removeAndSet(AD9834_MODE_MASK & AD9834_OPBITEN_MASK);
	else if (mode == 1)
		addAndRemoveAndSet(AD9834_MODE_ENABLE,AD9834_OPBITEN_MASK);
	else if (mode == 2)
		addAndRemoveAndSet(AD9834_OPBITEN_ENABLE | AD9834_DIV_2_ENABLE,AD9834_MODE_MASK);
}

// sign_mode: AD9834_SIGN_MODE_DAC_MSB_2, AD9834_SIGN_MODE_DAC_MSB, AD9834_SIGN_MODE_COMPARE_OUT
void AD9834_setSignOutput(uint8_t sign_mode) {
	if (!sign_mode)
		addAndRemoveAndSet(AD9834_OPBITEN_ENABLE,AD9834_MODE_MASK & AD9834_SIGN_PIB_MASK & AD9834_DIV_2_MASK);
	else if (sign_mode == 1) addAndRemoveAndSet(AD9834_OPBITEN_ENABLE | AD9834_DIV_2_ENABLE,AD9834_MODE_MASK & AD9834_SIGN_PIB_MASK);
	else if (sign_mode == 2) addAndRemoveAndSet(AD9834_OPBITEN_ENABLE | AD9834_DIV_2_ENABLE | AD9834_SIGN_PIB_ENABLE,AD9834_MODE_MASK);
}

void AD9834_setOutputChannel(uint8_t freq,uint8_t phase) {
	if (freq)
		AD9834_FS_HIGH();
	else AD9834_FS_LOW();
	if (phase)
		AD9834_PS_HIGH();
	else AD9834_PS_LOW();
}