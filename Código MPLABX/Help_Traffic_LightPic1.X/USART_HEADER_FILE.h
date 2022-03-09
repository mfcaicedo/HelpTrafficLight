/*!
\file   USART_HEADER_FILE.h
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
PIC18F4550 USART Header File 
http://www.electronicwings.com
******************************************************************************
*/
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef USART_HEADER_FILE_H
#define	USART_HEADER_FILE_H

#include <pic18f4550.h>
#include <xc.h>
void USART_Init(long);
void USART_TxChar(char);
void USART_SendString(const char *);
char USART_RxChar();

#define F_CPU 8000000/64
#define Baud_value (((float)(F_CPU)/(float)baud_rate)-1)

#endif	/* USART_HEADER_FILE_H */


