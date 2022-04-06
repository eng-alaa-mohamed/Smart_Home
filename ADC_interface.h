/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     ADC                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 14-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

typedef struct
{
	u8* Channel;   //pointer to channel
	u16* Result;  //pointer to array that holds ADC result of each channel conversion
	u8 Size;    // size or number of channels
	void (*NotificationFunc)(void);
}Chain_t;
/* function prototype for chain conversion - multiple analog channels */
u8 ADC_u8StartChainAsynch(Chain_t* Copy_Chain);



void ADC_voidInit(void);

u8 ADC_u8StartConversionSynch(u8 Copy_u8Channel, u16* Copy_pu16Reading);

// Global Interrupt must be enabled for this function //
u8 ADC_u8StartConversionAsynch(u8 Copy_u8Channel, u16* Copy_pu16Reading, void(*Copy_pvNotificationFunc)(void));
/* Single Ended Analog Input - One pin/wire */
#define SINGLE_ENDED_ADC0						 0
#define SINGLE_ENDED_ADC1						 1
#define SINGLE_ENDED_ADC2						 2
#define SINGLE_ENDED_ADC3						 3
#define SINGLE_ENDED_ADC4						 4
#define SINGLE_ENDED_ADC5						 5
#define SINGLE_ENDED_ADC6						 6
#define SINGLE_ENDED_ADC7						 7
#define SINGLE_ENDED_1_22V						30
#define SINGLE_ENDED_GND						31

/* Differential Analog Input - 2 pins/wires */
#define POSITIVE_ADC0_NEGATIVE_ADC0_GAIN_X10     8


#endif
