/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     USART              ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 19-03-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_


void USART_voidInit(void);
void USART_voidSend(u8 Copy_u8data);
u8 USART_u8Receive(void);

void USART_voidSendString(const char* Copy_pcString);

const char* USART_voidReceiveString(void);

void UART_Send_String(char* st_pt);
void UART_Receive_String(u8 *command);

void gets_UART1(unsigned char *string);

void enable_UART_Int(void);

#endif /* USART_INTERFACE_H_ */
