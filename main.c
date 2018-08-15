/*
 * FlowerWatering.c
 *
 * Created: 15.08.2018 18:36:57
 * Author : Michal Trojnarski, Maria Mierzwa
 */ 

#define F_CPU 4800000

#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>

#define REF_AVCC (0 << REFS0)
#define REF_INT	 (1 << REFS0)
#define LEDY PB4
#define LEDG PB1
#define ADC_PORT 1

void displayValue(uint8_t value);
void initADC();
uint16_t readFromADC(uint8_t adc_num);

int main(void)
{
	DDRB |= (1 << LEDY) | (1 << LEDG);
	uint16_t readedValue = 0;
	//initADC();

	
    while (1) 
    {
		readedValue = readFromADC(ADC_PORT) / 10.24;
		uint8_t val = 99 - (readedValue & 0xFF);
		if (val < 30)
		{
			PORTB &= ~(1 << LEDY);
			PORTB &= ~(1 << LEDG);
			PORTB |= (1 << LEDY) | (1 << LEDG);
		}
		if ((val >= 30) && (val < 80))
		{
			PORTB &= ~(1 << LEDY);
			PORTB &= ~(1 << LEDG);
			PORTB |= (1 << LEDY);
		}
		if (val >= 80)
		{
			PORTB &= ~(1 << LEDY);
			PORTB &= ~(1 << LEDG);
			PORTB |= (1 << LEDG);
		}
    }
}



void initADC()
{
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// 128 prescale for 8MHz
	ADCSRA |= (1 << ADEN);	// enable ADC
}

uint16_t readFromADC(uint8_t adc_num)
{
	uint8_t readed_value;

	//ADMUX = adc_num;
	ADMUX |= (1 << MUX0);	// use AVCC as the reference
	ADMUX &= ~(1 << ADLAR); // right-adjusted
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);	// 128 prescale for 8MHz
	ADCSRA |= (1 << ADEN);	// enable ADC
	ADCSRA |= (1 << ADSC);	// starting conversion
	while (ADCSRA & (1 << ADSC)) {}	// wait until conversion is over

	
	readed_value = ADCL;
	readed_value = (ADCH << 8) + readed_value;

	return readed_value;
}