/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   Application        ****************/
/****************     SWC:     USART              ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 4-04-2022            ****************/
/*****************************************************************/
/*****************************************************************/
#include <string.h>
#include <util/delay.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "PORT_interface.h"
#include "GIE_interface.h"
#include "ADC_interface.h"
#include "ADC_register.h"
#include "TIMER_interface.h"
#include "TIMER_register.h"
#include "USART_interface.h"
#include "USART_register.h"
void cursor(void);
void get_input(void);
void pin_check (void);
char password[]="1234";
char lighton[]="lighton";
char lightoff[]="lightoff";
char fanon[]="fanon";
char fanoff[]="fanoff";
char opendoor[]="opendoor";
char closedoor[]="closedoor";
char smartlighton[]="smartlighton";
char smartlightoff[]="smartlightoff";
char smartfanon[]="smartfanon";
char smartfanoff[]="smartfanoff";

char exit[]="exit";
int Str[8];

u8 light_flag=0;
u8 fan_flag=0;
void main (void)
{
	PORT_voidInit();      //Rx input (PD0) and Tx output (PD1)
	GIE_voidEnable();     // enable global interrupt
	ADC_voidInit();
	u16 App_u16ADCReading_light;
	u16 App_u16ADCReading_fan;
	TIMER1_voidInit();
	TIMER0_voidInit();
	ICR1 = 19999;
	OCR1B = 0;
	TIMER0_voidSetCompareMatchValue(0);
	USART_voidInit();
	USART_voidSend('\n');
	USART_voidSend('\r');
	UART_Send_String("UART Module Initialized and Active");
	USART_voidSend('\n');
	USART_voidSend('\r');
	pin_check();
	enable_UART_Int();

	while(1)
	{

		ADC_u8StartConversionSynch(SINGLE_ENDED_ADC3, &App_u16ADCReading_fan);  //ADC Synchronous mode - thermal sensor
		ADC_u8StartConversionSynch(SINGLE_ENDED_ADC2, &App_u16ADCReading_light);  //ADC Synchronous mode - LDR
		if (fan_flag==1)
		{
			TIMER0_voidSetCompareMatchValue(App_u16ADCReading_fan/4);   //output is on OC0 which is pin PB3
		}
		else if (fan_flag==0)
		{
			TIMER0_voidSetCompareMatchValue(0);    //output is on OC0 which is pin PB3
		}
		if (light_flag==1)
		{
			OCR1B = App_u16ADCReading_light-384;   //output is on OC1B which is pin PD4
		}

		else if(light_flag==0)
		{
			OCR1B = 0;                       //output is on OC1B which is pin PD4
		}

		UARTINT_voidSetCallBack(&get_input);
	}
}

void get_input(void)
{
	gets_UART1(Str);
	if((strncmp(Str, fanon, 5))==0)
	{
		DIO_u8SetPinValue (DIO_u8PORTA, DIO_u8PIN1, DIO_u8PIN_HIGH); //fan ON;
		cursor();
	}

	else if((strncmp(Str, fanoff, 6))==0)
	{
		DIO_u8SetPinValue (DIO_u8PORTA, DIO_u8PIN1, DIO_u8PIN_LOW); //fan OFF
		cursor();
	}

	else if((strncmp(Str, lighton, 7))==0)
	{
		DIO_u8SetPinValue (DIO_u8PORTA, DIO_u8PIN0, DIO_u8PIN_HIGH); //Light ON;
		cursor();
	}

	else if((strncmp(Str, lightoff, 8))==0)
	{
		DIO_u8SetPinValue (DIO_u8PORTA, DIO_u8PIN0, DIO_u8PIN_LOW); //Light OFF;
		cursor();
	}
	else if((strncmp(Str, opendoor, 8))==0)
	{
		Servo_Motor(90); //open door;
		cursor();
	}
	else if((strncmp(Str, closedoor, 9))==0)
	{
		Servo_Motor(0); //close door;
		cursor();
	}
	else if((strncmp(Str, smartlighton, 12))==0)
	{
		light_flag=1;
		cursor();
	}
	else if((strncmp(Str, smartlightoff, 13))==0)
	{
		light_flag=0;
		cursor();
	}
	else if((strncmp(Str, smartfanon, 10))==0)
	{
		fan_flag=1;
		cursor();
	}
	else if((strncmp(Str, smartfanoff, 10))==0)
	{
		fan_flag=0;
		cursor();
	}
	else if((strncmp(Str, exit, 4))==0)
	{
		DIO_u8SetPinValue (DIO_u8PORTA, DIO_u8PIN1, DIO_u8PIN_LOW); //fan OFF
		DIO_u8SetPinValue (DIO_u8PORTA, DIO_u8PIN0, DIO_u8PIN_LOW); //Light OFF;
		Servo_Motor(0); //close door;
		fan_flag=0;
		light_flag=0;
		USART_voidSend('\n');
		USART_voidSend('\r');
		UART_Send_String("UART Module Initialized and Active");
		USART_voidSend('\n');
		USART_voidSend('\r');
		pin_check();
	}
	else
	{
		USART_voidSend('\r');        //ASCII value 10 is used for carriage return (to print a new line)
		USART_voidSend('\n');
		UART_Send_String("Wrong command");
		USART_voidSend('\r');        //ASCII value 10 is used for carriage return (to print a new line)
		USART_voidSend('\n');
		UART_Send_String("Smart_Home>>");
	}
}


void pin_check (void)
{
	u8 trials=0;
	do
	{
		UART_Send_String("Enter PIN: ");
		gets_UART1(Str);   //get password
		if((strncmp(Str, password, 4))==0)
		{
			UART_Send_String("Welcome to Smart Home Application");
			USART_voidSend('\n');
			USART_voidSend('\r');
			UART_Send_String("                            List of commands             ");
			USART_voidSend('\n');
			USART_voidSend('\r');
			UART_Send_String("lighton           lightoff         opendoor       closedoor      fanon    fanoff");
			USART_voidSend('\n');
			USART_voidSend('\r');
			UART_Send_String("smartlighton      smartlighton     smartfanon     smartfanoff     exit");
			USART_voidSend('\n');
			USART_voidSend('\r');
			UART_Send_String("Smart_Home>>");
			break;
		}
		else
		{
			trials++;
			UART_Send_String("Wrong password, please try again - remaining trials: ");
			USART_voidSend('\n');
			USART_voidSend('\r');
		}
	}while(trials<3);

	if (trials==3)
	{
		USART_voidSend('\n');
		USART_voidSend('\r');
		UART_Send_String("Account locked !!");
		for(;;);
	}
}

/* Servo Motor (Limited) function using Timer1
 * Argument: angle in degrees between 0 to 180  */
void Servo_Motor(u8 rotation_angle)
{
	ICR1 = 19999;
	OCR1A = 449 + ((2050/180) * rotation_angle);   //output is on OC1A which is pin PD5
}
void cursor(void)
{
	USART_voidSend('\r');        //ASCII value 10 is used for carriage return (to print a new line)
	USART_voidSend('\n');
	UART_Send_String("Smart_Home>>");
}

