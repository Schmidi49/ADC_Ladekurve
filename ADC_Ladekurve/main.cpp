/******************************************************************************
Testprogram for reading ADC-Channels. Output of the ADC-Voltages on terminal
*******************************************************************************/
#include <stdio.h>
#include "K:\DriverLibHW\DriverLibHW\ADC.h"
#include "K:\DriverLibHW\DriverLibHW\USART.h"
#include "K:\DriverLibHW\DriverLibHW\Wait.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino.h>


#define CHARGEPIN 0
#define DISCHARGEPIN 1
#define ADC_CHANNEL 0
#define SAMPLING_RATE 50
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
		printf("%i\n", adc_value);
		WaitMs(SAMPLING_RATE);	//Antastrate 
		//i++;
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
		printf("%i\n", adc_value);
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

	USARTInit(0, 9600, 1, 0, 1, 0);

	discharge();	//Kondensator am Anfang entladen

	while(1) 
	{
		charge();  //lädt den Kondensator und schickt kontinuierlich werte
		discharge();

		WaitMs(5000);
	}

	return 0;

} // main