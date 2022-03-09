/*!
\file   USART_SOURCE_FILE.c
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

\par 
PIC18F4550 USART Source File 
http://www.electronicwings.com
******************************************************************************
*/
#include "USART_Header_File.h"
/*F**************************************************************************
* NAME: USART_Init
*----------------------------------------------------------------------------
* PARAMS:
* baud_rate: Tasa de baudios 
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method that initializes the USART
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void USART_Init(long baud_rate)
{
    float temp;
    TRISC6=0;                       /*Make Tx pin as output*/
    TRISC7=1;                       /*Make Rx pin as input*/
    temp=Baud_value;     
    SPBRG=(int)temp;                /*baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    TXSTA=0x20;                     /*Transmit Enable(TX) enable*/ 
    RCSTA=0x90;                     /*Receive Enable(RX) enable and serial port enable */
}
/*F**************************************************************************
* NAME: USART_TxChar
*----------------------------------------------------------------------------
* PARAMS:
* out: salida caracter 
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method that transmits the data(charaters)
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void USART_TxChar(char out)
{        
        while(TXIF==0);            /*wait for transmit interrupt flag*/
        TXREG=out;                 /*wait for transmit interrupt flag to set which indicates TXREG is ready
                                    for another transmission*/    
}
/*F**************************************************************************
* NAME: USART_RxChar
*----------------------------------------------------------------------------
* PARAMS:
* none
* return:  RCREG register return to main program. 
*----------------------------------------------------------------------------
* PURPOSE:
* Method that receives the data(charaters)
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
char USART_RxChar()
{
    while(RCIF==0);                 /*wait for receive interrupt flag*/
    if(RCSTAbits.OERR)
    {           
        CREN = 0;
        NOP();
        CREN=1;
    }
    return(RCREG);                  /*receive data is stored in RCREG register and return to main program */
}
/*F**************************************************************************
* NAME: USART_SendString
*----------------------------------------------------------------------------
* PARAMS:
* *out: puntero de caracteres
* return: none
*----------------------------------------------------------------------------
* PURPOSE:
* Method that sends the data(characters).
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void USART_SendString(const char *out)
{
   while(*out!='\0')
   {            
        USART_TxChar(*out);
        out++;
   }
}
