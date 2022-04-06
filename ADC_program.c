/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     ADC                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 14-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"
#include "ADC_register.h"

/* Global pointer to function to hold AD Interrupt address */
void (*ADC_pvISRFunc)(void) = NULL;

static u16* ADC_pu16Reading = NULL;
static void(*ADC_pvCallBackNotificationFunc)(void)=NULL;    //is used with Asynchronous or chain conversion

/*Define global variables that will be used by ISR
 * static because this variable (pointer) will be used only in this file */
static u8* ADC_pu8ChainChannelArr= NULL;      /*global variable to carry chain array */
static u8 ADC_u8ChainSize;                    /*global variable to carry number of channels */
static u16* ADC_pu16ChainResultArr=NULL;      /*global variable to carry chain result */
static u8 ADC_u8ChainConversionIndex=0;       /*global variable to carry the current conversion index */
static u8 ADC_u8ISRSource;                    /*global variable to carry which Async is used one channel or several channels */


u8 ADC_u8BusyState = IDLE;

u8 ADC_u8StartConversionSynch(u8 Copy_u8Channel, u16* Copy_pu16Reading)
{
	u32 Local_u32Counter=0;
	u8 Local_u8ErrorState=OK;
	if(ADC_u8BusyState==IDLE)
	{
		ADC_u8BusyState = BUSY;
		/*Clear the MUX bits in ADMUX register*/
		ADMUX&= MUX_Clear;
		/*Set the required channel into the MUX bits*/
		ADMUX|= Copy_u8Channel;  //need to update config file with all options

		/*Start conversion */
		SET_BIT(ADCSRA,ADCSRA_ADSC);

		/*Polling (busy waiting) until the conversion complete flag is set -
		 * but this way doesn't consider the time-out problem, which may occur if ADC stopped for any reason*
		 * *Polling (busy waiting) until the conversion complete flag is set or counter reaching timeout */
		while(((GET_BIT(ADCSRA,ADCSRA_ADIF))==0) && (Local_u32Counter != ADC_u32TIMEOUT) )   //keep in this line while flag=0  means conversion did not happened yet
		{
			Local_u32Counter ++;
		}
		if (Local_u32Counter == ADC_u32TIMEOUT)   //loop is broken due to time out is reached
		{
			Local_u8ErrorState=NOK;//as we need to return 2 things reading and error state as a result return reading by pointer
		}
		else  //Loop is broken due to flag is raised
		{
			/*clear the conversion complete flag*/
			SET_BIT(ADCSRA,ADCSRA_ADIF);
			/*Return the reading */
			if (Resolution == 8)
				*Copy_pu16Reading = ADCH;   //DC for 8bit
			else if (Resolution == 10)
				*Copy_pu16Reading = ADC;   //DC for 10bit

			ADC_u8BusyState = IDLE;
		}
	}
	else
	{
		Local_u8ErrorState = BUSY_FUNC;
	}

	return Local_u8ErrorState;
}


u8 ADC_u8StartConversionAsynch(u8 Copy_u8Channel, u16* Copy_pu16Reading, void(*Copy_pvNotificationFunc)(void))
{
	u8 Local_u8ErrorState=OK;
	if(ADC_u8BusyState==IDLE)
	{

		if((Copy_pu16Reading == NULL) || (Copy_pvNotificationFunc == NULL))
		{
			Local_u8ErrorState = NULL_POINTER;
		}
		else
		{
			/*Make ADC busy in order not to work until being idle*/
			ADC_u8BusyState = BUSY;   /*Adding this variable protected the function and make it reentrant function*/
			/*make ISR Source: single channel Asynchronous*/
			ADC_u8ISRSource= SINGLE_CHANNEL_ASYNCH;
			/* Initialize the reading variable globally */
			ADC_pu16Reading = Copy_pu16Reading;
			/* Initialize the callback notification function globally */
			ADC_pvCallBackNotificationFunc = Copy_pvNotificationFunc;

			/*Clear the MUX bits in ADMUX register*/
			ADMUX&= MUX_Clear;
			/*Set the required channel into the MUX bits*/
			ADMUX|= Copy_u8Channel;  //need to update config file with all options
			/*Start conversion */
			SET_BIT(ADCSRA,ADCSRA_ADSC);
			/* enable ADC interrupt */
			SET_BIT(ADCSRA,ADCSRA_ADIE);
		}
	}
	else
	{
		Local_u8ErrorState = BUSY_FUNC;
	}
	return Local_u8ErrorState;
}

void __vector_16 (void) __attribute__((signal));
void __vector_16 (void)
{
	if(ADC_u8ISRSource==SINGLE_CHANNEL_ASYNCH)
	{
		/*Read ADC result*/
		if (Resolution == 8)
			*ADC_pu16Reading = ADCH;   //DC for 8bit
		else if (Resolution == 10)
			*ADC_pu16Reading = ADC;   //DC for 10bit

		/*Make ADC state be IDLE due to finishing conversion*/
		ADC_u8BusyState = IDLE;
		/*Invoke the Callback notification function */
		ADC_pvCallBackNotificationFunc();  //to notify the user (main.c) that Digital reading is ready
		/*Disable ADC conversion complete interrupt*/
		CLR_BIT(ADCSRA,ADCSRA_ADIE);
	}
	else if(ADC_u8ISRSource==CHAIN_ASYNCH)
	{
        /* Read the current conversion */
       ADC_pu16ChainResultArr[ADC_u8ChainConversionIndex] = ADCH;    //for 8-bitresolution #if could be done for the 2 resolutions
       ADC_u8ChainConversionIndex++;  //increment index
       /*check whether chain is finished or not */
       if(ADC_u8ChainConversionIndex == ADC_u8ChainSize)
       {
    	   /*Chain is finished */
    	   ADC_u8BusyState = IDLE;   /*ADC is now IDLE */

    	   /* Invoke the callback of notification function */
    	   ADC_pvCallBackNotificationFunc();  /*this pointer to function could be changed to get u8 as input argument instead
    	   /* of void and pass ADCH value to it and in this case remove *result from struct
    	    * - in main.c get ADC value through this function*/
    	   /*Disable ADC conversion complete interrupt */
    	   CLR_BIT(ADCSRA,ADCSRA_ADIE);
       }
       else
       {
           /*Chain is not finished*/
    	   /*Set new required channel */
    	   ADMUX&= MUX_Clear; /*Clear the MUX bits in ADMUX register 0b11100000*/
    	   /*Set the required channel into the MUX bits*/
		   ADMUX|= ADC_pu8ChainChannelArr[ADC_u8ChainConversionIndex];  //need to update config file with all options
		   /*Start conversion*/
		   SET_BIT(ADCSRA,ADCSRA_ADSC);
       }
	}
}


// ADC initiation function for pre-build configuration //
void ADC_voidInit(void)
{
#if (Reference_Voltage == AVCC)
	SET_BIT(ADMUX,ADMUX_REFS0);
	CLR_BIT(ADMUX,ADMUX_REFS1);

#elif (Reference_Voltage == Half_AVCC)
	SET_BIT(ADMUX,ADMUX_REFS0);
	SET_BIT(ADMUX,ADMUX_REFS1);

#elif (Reference_Voltage == External_AVCC)
	CLR_BIT(ADMUX,ADMUX_REFS0);
	CLR_BIT(ADMUX,ADMUX_REFS1);
#else
#error "Wrong reference voltage"
#endif

#if (Resolution == 8)  //Left Adjust Result
	SET_BIT(ADMUX,ADMUX_ADLAR);
#elif (Resolution == 10)
	CLR_BIT(ADMUX,ADMUX_ADLAR);
#else
#error "Wrong resolution"
#endif

	/*Enable peripheral*/
	SET_BIT(ADCSRA,ADCSRA_ADEN);

	/*Set prescaler - These 3 bits determine the division factor between the XTAL frequency and the input clock to the ADC.*/
#if (ADC_PRESC==128)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 128;
#elif (ADC_PRESC==64)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 6;
#elif (ADC_PRESC==32)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 5;
#elif (ADC_PRESC==16)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 4;
#elif (ADC_PRESC==8)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 3;
#elif (ADC_PRESC==4)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 2;
#elif (ADC_PRESC==2)
	ADCSRA &= ADC_PRESC_MASK;    //clear first 3 bits of ADCSRA register
	ADCSRA |= 1;
#else
#error "Wrong prescaler"
#endif

#if (ADC_INTERRUPT == 1)
	SET_BIT(ADCSRA,ADCSRA_ADIE);
#elif (ADC_INTERRUPT == 0)
	CLR_BIT(ADCSRA,ADCSRA_ADIE);
#else
#error "Wrong selection"
#endif

#if Auto_Trigger_Enable == 1
	SET_BIT(ADCSRA,ADCSRA_ADATE);
#elif Auto_Trigger_Enable == 0
	CLR_BIT(ADCSRA,ADCSRA_ADATE);
#else
#error "Wrong selection"
#endif

#if (ADC_TRIG_SRC == 0)                  //ADC Trigger Source
	SFIOR &= TRIG_SOURCE_MASK;         //clear the last 3 bits SFIOR register
	SFIOR |= 0;
#elif ADC_TRIG_SRC == 1
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 1;
#elif ADC_TRIG_SRC == 2
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 2;
#elif ADC_TRIG_SRC == 3
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 3;
#elif ADC_TRIG_SRC == 4
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 4;
#elif ADC_TRIG_SRC == 5
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 5;
#elif ADC_TRIG_SRC == 6
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 6;
#elif ADC_TRIG_SRC == 7
	SFIOR &= TRIG_SOURCE_MASK;
	SFIOR |= 7;
	//#else
	//#error "Wrong selection"
#endif

}

/* Chain conversion to convert several analog signals at the same time using Asynchronous mode
 * Function argument is a struct defines in _interface file */
u8 ADC_u8StartChainAsynch(Chain_t* Copy_Chain)
{
	u8 Local_u8ErrorState=OK;
	/*check NULL pointer - if function argument is a pointer then check for null value  */
	if (Copy_Chain == NULL)
	{
		Local_u8ErrorState= NULL_POINTER;
	}
	else
	{
		if(ADC_u8BusyState == IDLE)
		{
			ADC_u8BusyState = BUSY;   //make ADC busy
			/*make ISR Source: Chain Asynchronous*/
			ADC_u8ISRSource= CHAIN_ASYNCH;
			/* Initialize chain channel array - */
			ADC_pu8ChainChannelArr = Copy_Chain->Channel;    //convert local variables (struct) to global variables
			/* Initialize chain result array - */
			ADC_pu16ChainResultArr = Copy_Chain->Result;
			/* Initialize Chain size - */
			ADC_u8ChainSize = Copy_Chain->Size ;
			/*Initialize Notification function */
			ADC_pvCallBackNotificationFunc = Copy_Chain->NotificationFunc;

			/*Initialize current conversion index */
			ADC_u8ChainConversionIndex=0;

			/*Set required channel */
			ADMUX&= MUX_Clear; /*Clear the MUX bits in ADMUX register 0b11100000*/
		    /*Set the required channel into the MUX bits*/
			ADMUX|= ADC_pu8ChainChannelArr[ADC_u8ChainConversionIndex];  //need to update config file with all options
			/*Start conversion*/
			SET_BIT(ADCSRA,ADCSRA_ADSC);
			/*Enable conversion complete interrupt */
			SET_BIT(ADCSRA,ADCSRA_ADIE);
		}
		else
		{
			Local_u8ErrorState= BUSY_FUNC;
		}
	}
	return Local_u8ErrorState;
}







