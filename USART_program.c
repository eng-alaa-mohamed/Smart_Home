/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     USART              ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 19-03-2022             ****************/
/*****************************************************************/
/*****************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "USART_register.h"
#include "USART_interface.h"
#include "USART_config.h"
void UART_voidWriteNumber(u16 Copy_u16Number);
static u16 Private_GetPower(u8 Num1, u8 Num2);
void UARTINT_voidSetCallBack(void(*Copy_pfun)(void));
void USART_voidInit(void)
{
	/*Because UCSRC register can not be configured the regular way i.e. SET_BIT & CLR-BIT but it must
	 * be set once, so as a workaround, define a local variable and assign bits as required then equal
	 * this variable to the register */
	u8 Local_u8UCSRC_Value = 0;
	SET_BIT(Local_u8UCSRC_Value,7);             //use UCSRC register (if zero, use UBRRH register)
	/*Asynchronous UART*/
	CLR_BIT(Local_u8UCSRC_Value,UCSRC_UMSEL);

	/*Select Parity to disable*/
	CLR_BIT(Local_u8UCSRC_Value,UCSRC_UPM0);
	CLR_BIT(Local_u8UCSRC_Value,UCSRC_UPM1);
	CLR_BIT(Local_u8UCSRC_Value,UCSRC_USBS);   //1 stop bit

	/*Set frame size to 8 bits*/
	SET_BIT(Local_u8UCSRC_Value,UCSRC_UCSZ1);  //set 1 to bit 8
	SET_BIT(Local_u8UCSRC_Value,UCSRC_UCSZ0);  //set 1 to bit 8

	UCSRC=Local_u8UCSRC_Value;
	UBRRL=Baudrate;                   //baud rate = 9600 bit/sec

	/*Enable Tx & Rx*/
	SET_BIT(UCSRB,UCSRB_RXEN);
	SET_BIT(UCSRB,UCSRB_TXEN);


}


void USART_voidSend(u8 Copy_u8data)
{
	/*flag is raised when buffer register is ready to send data*/
	while(GET_BIT(UCSRA,UCSRA_UDRE)==0);  //Polling - wait until transmit buffer is empty -
	UDR=Copy_u8data;
}

void UART_Send_String(char* st_pt)
{
	while(*st_pt)
		USART_voidSend(*st_pt++);
}

void UART_Receive_String(u8 *command)
{
	char counter=0;
	while(*command)
	{
		counter++;
		//(command+counter) = USART_voidReceive();
	}

}

u8 USART_u8Receive(void)
{
	while(GET_BIT(UCSRA,UCSRA_RXC)==0);
	return UDR;
}


const char* USART_voidReceiveString(void)
{
	u8 local_u8Counter=0;
	const char* Copy_pcString;
	while(GET_BIT(UCSRA,UCSRA_RXC)==0);

	while(Copy_pcString[local_u8Counter]!= '\0')//an array containing the characters and terminated with a '\0' character
		//String is array of characters and last element is null character
	{
		USART_voidSend(Copy_pcString[local_u8Counter]);
		local_u8Counter++;
	}
	UDR = *Copy_pcString;
	return Copy_pcString;
}


void USART_voidSendString(const char* Copy_pcString)

{
	u8 local_u8Counter=0;
	while(GET_BIT(UCSRA,UCSRA_UDRE)==0);
	while(Copy_pcString[local_u8Counter]!= '\0')//an array containing the characters and terminated with a '\0' character
		//String is array of characters and last element is null character
	{
		USART_voidSend(Copy_pcString[local_u8Counter]);
		local_u8Counter++;
	}
	UDR= *Copy_pcString;
}

void gets_UART1(unsigned char *string)  //Receive a character until carriage return or newline
{
	unsigned char i=0,J=0;
	do
	{
		string[i]= USART_u8Receive();
		J = string[i];
		i++;
	}
	while((J!='\0') && (J!='\r'));  //while no Line Feed or Carriage Return
}


/*
void gets_UART1(unsigned char *string)  //Receive a character until carriage return or newline
{
	unsigned char i=0,J=0;
	do
	{
 *(string+i)= USART_u8Receive();
		J = *(string+i);
		i++;
	}
	while((J!='\n') && (J!='\r'));  //while no Line Feed or Carriage Return
	i++;
 *(string+i) = '\0';
}
 */
static void (*UARTINT_ApfuncUARTINT)(void);
void UARTINT_voidSetCallBack(void(*Copy_pfun)(void))
{
	UARTINT_ApfuncUARTINT = Copy_pfun;
}
//USART, Rx Complete
void __vector_13 (void) __attribute__((signal));
void __vector_13 (void)
{
	UARTINT_ApfuncUARTINT();
}

/*Enable Rx Interrupt*/
void enable_UART_Int(void)
{
	SET_BIT(UCSRB,UCSRB_RX); //RXCIE: RX Complete Interrupt Enable
	SET_BIT(UCSRA,UCSRA_RXC); //RXCIE: RX Complete Interrupt Enable
}

void UART_voidWriteNumber(u16 Copy_u16Number)
{
	u8 Counter=0,Digits=0,Current;
	u16 CopyNumber=Copy_u16Number;
	while(CopyNumber)
	{
		CopyNumber/=10;
		Digits++;
	}
	CopyNumber=Copy_u16Number;
	for(Counter=0;Counter<Digits;Counter++)
	{
		Current=CopyNumber/(Private_GetPower(10,Digits-1-Counter));
		//CLCD_voidSendData(Current+'0');
		USART_voidSend(Current+'0');
		CopyNumber%=(Private_GetPower(10,Digits-1-Counter));
	}

}
static u16 Private_GetPower(u8 Num1, u8 Num2)
{
	u16 Result=1;
	u8 Counter=0;
	for(Counter=0;Counter<Num2;Counter++)
	{
		Result*=Num1;
	}
	return Result;
}
