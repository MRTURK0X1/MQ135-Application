#include <MQ135.h>

void MQ135_Init(MQ135*mq135,ADC_HandleTypeDef*adc,uint8_t temp,uint8_t hum){

	mq135->temp = temp;
	mq135->hum = hum;
	mq135->ADC = adc;
	mq135->R0 = getR0(mq135);

}

void update_tempHum(MQ135*mq135,uint8_t temp,uint8_t hum){
	mq135->temp = temp;
	mq135->hum = hum;
}

float getVoltage(ADC_HandleTypeDef* hadc) {

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 100);
	uint16_t adcRead = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);

	return  (float)adcRead * Vstep; // PRODUCT With v step

}

float getCorrectionFactor(MQ135*mq135) {
    return (CorrA * pow(mq135->temp, 3) + CorrB * pow(mq135->temp, 2) - CorrC * mq135->temp + CorrD - (CorrE * mq135->hum - CorrE * 33));
}

float getResistance(MQ135*mq135) {
    float voltage = getVoltage(mq135->ADC);
    float rs = ((5 * RL) / voltage) - (float)RL;
    if (rs < 0) {
        rs = 0;
    }
    return rs;
}

float getCorrectedResistance(MQ135*mq135) {
    return getResistance(mq135) / getCorrectionFactor(mq135);
}


float getR0(MQ135*mq135) {
    float r0 = getResistance(mq135) / 3.6;
    return r0;
}

float getCorrectedR0(MQ135*mq135) {
    float r0 = getCorrectedResistance(mq135) / 3.6;
    return r0;
}


float getPPM(MQ135*mq135,float m,float b) {
    float ratio = getResistance(mq135) / mq135->R0;
    float ppm_log = (log10(ratio) - b) / m;
    float ppm = pow(10, ppm_log);
    if (ppm < 0) {
        ppm = 0;
    }
    return ppm;
}

float getCorrectedPPM(MQ135*mq135,float m,float b) {
    float ratio = getCorrectedResistance(mq135) / mq135->R0;
    float ppm_log = (log10(ratio) - b) / m;
    float ppm = pow(10, ppm_log);
    if (ppm < 0) {
        ppm = 0;
    }
    return ppm;
}


float getCO2(MQ135*mq135) {
    // return getPPMLinear(-0.3525, 0.7142) + ATMOCO2;
    return getPPM(mq135,110.47, -2.862) + ATMOCO2;
}

float getCorrectedCO2(MQ135*mq135) {
    return getCorrectedPPM(mq135,110.47, -2.862) + ATMOCO2;
}

float getAlcohol(MQ135*mq135) {
    return getPPM(mq135,77.255, -3.18);
}

float getCorrectedAlcohol(MQ135*mq135) {
    return getCorrectedPPM(mq135,77.255, -3.18);
}

float getNH3(MQ135*mq135) {
    return getPPM(mq135,-0.4359,0.6078);
}

float getCorrectedNH3(MQ135*mq135) {
    return getCorrectedPPM(mq135,-0.4359,0.6078);
}

