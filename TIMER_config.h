/*****************************************************************/
/*****************************************************************/
/****************     Author:  Alaa Mohamed       ****************/
/****************     Layer:   MCAL               ****************/
/****************     SWC:     TIMER                ****************/
/****************     Version: 1.00               ****************/
/****************     Date: 23-02-2022             ****************/
/*****************************************************************/
/*****************************************************************/
#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

#define Normal_Mode                1   //overflow mode
#define CTC                        2   //Clear time on Compare match
#define Counter                    3
#define Fast_PWM                   4
#define Phase_Correct_PWM          5

#define Inverting                  1
#define Non_Inverting              2
//General Purpose Timer
/*Timer Mode>>               1: Normal_Mode
Clear time on Compare match  2: CTC
 	 	 	 	 	 	 	 3- Counter
                             4- Fast_PWM
							 5- Phase_Correct_PWM  */
#define Timer0_Mode      Fast_PWM


/*PWM Mode>>                 1: Inverting
 	 	 	 	 	 	 	 2: Non_Inverting        */
#define PWM_Mode         Non_Inverting


#endif
