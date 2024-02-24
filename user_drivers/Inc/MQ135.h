#ifndef MQ135_H
#define MQ135_H

#include "stm32f1xx_hal.h"
#include <math.h>
/// Resistor on Sensor in kΩ
#define RL 4.7

/// Voltage on Sensor in V
#define VIn 5

/// Board analog Input Resolution
/// Default: 2^10
#define Resolution 4096

/// CO2 Level in Atmosphere
#define ATMOCO2 397.13

//Correction Values
#define CorrA -0.000002469136
#define CorrB 0.00048148148
#define CorrC 0.0274074074
#define CorrD 1.37530864197
#define CorrE 0.0019230769

#define Vstep (float)VIn/(Resolution-1)



typedef struct _MQ135{

	ADC_HandleTypeDef* ADC;
	float R0;
	uint8_t temp;
	uint8_t hum;

}MQ135;

void MQ135_Init(MQ135*mq135,ADC_HandleTypeDef*adc,uint8_t temp,uint8_t hum);

void update_tempHum(MQ135*mq135,uint8_t temp,uint8_t hum);

/// Assume CO2 Level is the default Atmospheric Level (~400ppm)
float getR0(MQ135*mq135);
float getCorrectedR0(MQ135*mq135);

/// Gets the resolved sensor voltage
float getVoltage(ADC_HandleTypeDef* hadc);

/// Calculate Correction Factor depending on temparature and humidity
float getCorrectionFactor(MQ135*mq135);

/// Calculates the Resistance of the Sensor
float getResistance(MQ135*mq135);
float getCorrectedResistance(MQ135*mq135);
/// Calculates ppm on a exponential curve
/// (Different Gases have different curves)
float getPPM(MQ135*mq135, float m, float b);
float getCorrectedPPM(MQ135*mq135, float m, float b);

/// Gets ppm of ammonia in Air (NH3)
float getNH3(MQ135*mq135);
float getCorrectedNH3(MQ135*mq135);
/// Gets ppm of CO2 in Air
float getCO2(MQ135*mq135);
float getCorrectedCO2(MQ135*mq135);
/// Gets ppm of Alcohol in Air
float getAlcohol(MQ135*mq135);
float getCorrectedAlcohol(MQ135*mq135);


#endif

