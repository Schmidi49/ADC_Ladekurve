/******************************************************************************
Testprogram for reading ADC-Channels. Output of the ADC-Voltages on terminal
*******************************************************************************/
#include <stdio.h>
#include "ADC.h"
#include "USART.h"
#include "Wait.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino.h>
#include <Wire.h>



#define CHARGEPIN 0
#define DISCHARGEPIN 1
#define ADC_CHANNEL 0
#define SAMPLING_RATE 100
// from #include "ADC.h":
//#define ADC_VREF_TYPE_EXTERNAL_AVCC 0x40 // ADC voltage reference: AVCC pin

uint16_t adc_value = 1023;

void charge()
{
	int i = 0;
	PORTB |= 1 << CHARGEPIN;			//charge pin HIGH
	while (adc_value < 973)	//95% von 5V -> geladen
	{
		adc_value = ADCReadChannel(ADC_CHANNEL); //get adc value 
		//printf("Conversion %d: 0x%04x %4ldmV\n", i, adc_value, (int32_t)((int32_t)adc_value*(int32_t)5000 / (int32_t)1024)); //sending data to console
		Wire.write(adc_value);
		WaitMs(SAMPLING_RATE);	//Antastrate 
		i++;
	}
}

void discharge()
{
	DDRB |= (1 << DISCHARGEPIN);		//discharge pin OUTPUT
	PORTB &= ~(1 << CHARGEPIN);			//charge pin LOW
	PORTB &= ~(1 << DISCHARGEPIN);			//discharge pin LOW

	while (adc_value > 51)	//5% von 5V -> entladen
	{
		adc_value = ADCReadChannel(ADC_CHANNEL); //get adc value 
	}

	DDRB &= ~(1 << DISCHARGEPIN);	//discharge pin INPUT
}

int16_t main(void) 
{
	ADCInit(ADC_VREF_TYPE_EXTERNAL_AVCC); // init ADC!!!!

	//PORTA.0: charge pin
	//PORTA.1: discharge pin
	DDRB |= (1<<CHARGEPIN);		//charge pin OUTPUT
	DDRB &= ~(1 << DISCHARGEPIN);	//discharge pin INPUT
	PORTB &= ~(1 << CHARGEPIN);			//charge pin LOW

	USARTInit(0, 115200, 1, 0, 1, 0);

	Wire.begin(); // join i2c bus (address optional for master)

	discharge();	//Kondensator am Anfang entladen

	while(1) 
	{
		//printf("+\n");	//start des Werteübertragen (konsole)
		Wire.beginTransmission(0x08);
		charge();
		Wire.write(-1);
		Wire.endTransmission();
		//printf("-\n");	//stop des werteübertragens (konsole)
		discharge();

		WaitMs(10000);
	}

	return 0;

} // main