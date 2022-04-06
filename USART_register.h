/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     USART              ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 19-03-2022             ****************/
/*****************************************************************/
/*****************************************************************/

#ifndef USART_REGISTER_H_
#define USART_REGISTER_H_

#define UDR              *((volatile u8*)0x2C)          //USART I/O Data Register – UDR

#define UBRRH            *((volatile u8*)0x40)
#define UBRRL            *((volatile u8*)0x29)

#define UCSRA            *((volatile u8*)0x2B)          //USART Control and Status  Register A – UCSRA
#define UCSRA_RXC	     7
#define UCSRA_TXC	     6
#define UCSRA_UDRE	     5
#define UCSRA_FE	     4
#define UCSRA_DOR	     3
#define UCSRA_PE 	     2
#define UCSRA_U2X	     1
#define UCSRA_MPCM	     0


#define UCSRB     		 *((volatile u8*)0x2A)         //USART Control and StatusRegister B – UCSRB
#define UCSRB_RX         7
#define UCSRB_TX         6
#define UCSRB_UDRIE      5
#define UCSRB_RXEN       4
#define UCSRB_TXEN       3
#define UCSRB_UCSZ2      2
#define UCSRB_RXB8       1
#define UCSRB_TXB8       0




#define UCSRC            *((volatile u8*)0x40)         //USART Control and Status Register C – UCSRC
#define UCSRC_URSEL      7
#define UCSRC_UMSEL      6
#define UCSRC_UPM1       5
#define UCSRC_UPM0       4
#define UCSRC_USBS       3   //Stop bit select
#define UCSRC_UCSZ1      2
#define UCSRC_UCSZ0      1

#endif /* USART_REGISTER_H_ */
