/*!
\file   mainPic2.c
\date   2022-03-09
\author Milthon F Caicedo mfcaicedo@unicauca.edu.co,
 * Yazmin Gutierrez yavigutierrez@unicauca.edu.co, 
 * Erika Camacho erikalcamacho@unicauca.edu.co, 
 * Brandon Nicolas Bohórquez, bbohorquez@unicauca.edu.co 
 
\brief  Proyecto HelpTrafficLight.

\par Copyright
Information contained here in is proprietary to aforementioned authors and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2022. All rights reserved.
******************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic18f4550.h>
#include "fuses.h"
#include "USART_Header2.h"

#define _XTAL_FREQ 8000000
#define MOTORIN1 LATBbits.LB1
#define MOTORIN2 LATBbits.LB2
#define MOTORENABLE LATBbits.LB7
#define ALARMA LATD4

#define IN 1
#define OUT 0
#define ON 1
#define OFF 0
/*==============================================================================
 ===============================================================================*/
int main(int argc, char** argv) {
    //Settings
    OSCCON=0b01110010;
    ADCON1 = 0x0F;   
    TRISD = OUT;
    //Bluetooth
    char data_in;
    USART_Init(9600);         /* initialize USART operation with 9600 baud rate */ 
     //Settigns motor
    TRISBbits.RB0 = OFF; //M1
    TRISBbits.RB1 = OFF; 
    TRISBbits.RB7 = OFF; //Enable
    
    while(1){    
        data_in = USART_RxChar();   //Receive Characters
        if(data_in == 'A'){         //Activate vibration
            MOTORENABLE = ON;
            MOTORIN1 = ON;          //counterclockwise  movement
            MOTORIN2 = OFF; 
        }else if(data_in=='D'){     //Turn off vibration
            MOTORENABLE = OFF;
            MOTORIN1 = OFF;
            MOTORIN2 = OFF; 
        }else if(data_in=='U'){     //Activate alarm
            ALARMA=ON;
        }else if(data_in=='S'){     //Turn off alarm 
            ALARMA=OFF;
        }
    }
    return (EXIT_SUCCESS);
}