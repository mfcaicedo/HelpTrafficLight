/*!
\file   mainPic1.c
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
/*==========================================================================================================
 ===========================================================================================================*/
#include <xc.h>                     
#include <stdint.h>                
#include <stdio.h>            
#include <string.h>             
#include "fuses.h"
#include "LCD_16x2_Library.h"  
#include "USART_HEADER_FILE.h"

#define _XTAL_FREQ 8000000
#define INPUT 1
#define OUTPUT 0
#define ON 1
#define OFF 0
#define HIGH 1
#define LOW 0
//PUERTOS BARRERA
#define ledBarrera LATD3
//PUERTOS SENSOR PIR
#define sensorPirInto TRISA2
#define sensorPirReader PORTAbits.RA2
//PUERTOS ALARMA
#define alarma LATD4
//PUERTOS D
#define puertosD TRISD
//PUERTOS LEDS SEMAFORO
#define ledRed LATD0
#define ledRedLectura PORTDbits.RD0
#define ledYellow LATD1
#define ledYellowReader PORTDbits.RD1
#define ledGreen LATD2
#define ledGreenReader PORTDbits.RD2
//Ultrasonico
#define Pin_Trig    LATAbits.LATA0    //"Pin_Trig" RA0 (OUT).
#define Pin_Echo    PORTAbits.RA1     //"Pin_Echo" RA1 (IN).
/*==========================================================================================================*/
uint16_t LCD_Buffer [16];              // Arreglo para mostrar las variabes en la pantalla LCD 2x16.
/*==========================================================================================================*/
void controlSemaforo(void);
void funcion_SensorPir(int bandera);
void delaymsled_red_green(void);
void delaymsled_yellow(void);
void llamar_ultrasonico(uint16_t Distancia);
void Configuracion_Registros (void);  
void Timer1_Init (void);              
uint16_t Obtener_Distancia (void); 
/*==========================================================================================================
 ===========================================================================================================*/
int main(int argc, char** argv){                   
    //config
    OSCCON=0b01110010;
    ADCON1=0x0F;
    USART_Init(9600);
    Configuracion_Registros();        //Configuración de registros.
    Timer1_Init();                    //Timer1.
    lcd_init();                       //Pantalla LCD 2x16.
    
    while(1){
        controlSemaforo(); 
    }
    return (EXIT_SUCCESS);;
}
/*F**************************************************************************
* NAME: controlSemaforo
*----------------------------------------------------------------------------
* PARAMS:
* none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method that controls the Traffic Light
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void controlSemaforo(void){
    puertosD = OUTPUT;   // Puertos D --> OUT
    ledRed = ON;     //Secuencia semáforo: LedRed --> ON, LedYellow --> OFF, LedGreen --> OFF 
    uint16_t Distancia;
    llamar_ultrasonico(Distancia);
    __delay_ms(7);
    
    if(sensorPirReader == HIGH){ //Detección personas
        alarma = ON;
        USART_SendString("A");
    }
    
    delaymsled_red_green();
    ledRed = OFF;
    
     if(ledRedLectura == LOW){
        ledYellow = ON; //LedRed --> OFF, LedYellow --> ON, LedGreen --> OFF 
        llamar_ultrasonico(Distancia);
        alarma = OFF;
        USART_SendString("D");
        ledYellow = OFF;
        funcion_SensorPir(0);
    }
    
    if(ledYellowReader == LOW){ 
        ledGreen = ON;      //LedRed --> OFF, LedYellow --> OFF, LedGreen --> ON
        llamar_ultrasonico(Distancia);
        funcion_SensorPir(1);
        ledGreen = OFF;
        ledYellow = ON;
        USART_SendString("S");
        delaymsled_yellow();
        ledYellow = OFF;    
    }   
}
/*F**************************************************************************
* NAME: funcion_SensorPir
*----------------------------------------------------------------------------
* PARAMS:
* bandera: allows validating conditionals for the correct functioning of the method.
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to control the pir sensor, activate barrier and alarm. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void funcion_SensorPir(int bandera){
    sensorPirInto = INPUT;
    ledBarrera = OFF;    
    uint16_t Distancia;
    
    if(sensorPirReader == HIGH && ledGreenReader == HIGH){  
        ledBarrera = ON;  
        Distancia = Obtener_Distancia();
        
        if(Distancia >= 40 && Distancia <= 380){
            USART_SendString("U");
        }
        delaymsled_red_green();
        
        ledBarrera = OFF;
        ledGreen = OFF;
        bandera = 2; //validate input (if)
    }else{
        ledBarrera = OFF;
    }
    if(bandera == 0){ 
        ledGreen = ON;
    }else if(bandera != 2){
        ledGreen = ON;
        delaymsled_red_green();    
    }
}
/*F**************************************************************************
* NAME: delaymsled_red_green
*----------------------------------------------------------------------------
* PARAMS:
* none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to make the delays in the traffic light (led red and led green aprox. 12seg)
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void delaymsled_red_green(void){
    __delay_ms(1000);
    __delay_ms(1000);
    __delay_ms(1000);
    __delay_ms(1000);
}
/*F**************************************************************************
* NAME: delaymsled_yellow
*----------------------------------------------------------------------------
* PARAMS:
* none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to make the delays in the traffic light (led yellow aprox. 0.05 seg)
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void delaymsled_yellow(void){
    __delay_ms(500);
}
/*F**************************************************************************
* NAME: llamar_ultrasonico
*----------------------------------------------------------------------------
* PARAMS:
* Distancia: array of the obtained distance in the ultrasonic sensor
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to call the ultrasonic sensor and validate the distance of the
* blind person, depending on the response, the alarm is activated or not.
*----------------------------------------------------------------------------
* NOTE:
* El Sensor ultrasonico HC-SR04 trabaja a una frecuencia de 40KHz, mientras que el oido humano esta entre el rango de 20Hz a 20KHz.
* Rango de medición de 2 cm a 400 cm.
* Velocidad del sonido 340 m/s >>> 0.034cm/us   >>>>> (20*C).
* Formula: d=v*t  >>> Distancia = Velocidad del sonido*Tiempo de ida
* Distancia = (0.034cm/us)*(Tiempo/2)
* Distancia = 0.017*Tiempo de ida.
*****************************************************************************/
void llamar_ultrasonico(uint16_t Distancia){
    Distancia = Obtener_Distancia(); 
    lcd_gotoxy(1,1);              // Position:  row 1, column 1.
    lcd_putc(" INFO ULTRASONICO "); // Message to screen LCD.
    sprintf(LCD_Buffer,"Distancia: %03dcm", Distancia+1);//"Distancia" --> "LCD_Buffer".
    lcd_gotoxy(2,1);              //Position: row 2, column 1
    lcd_putc(LCD_Buffer);         //Show buffer_lcd
    __delay_ms(200);   
}
/*F**************************************************************************
* NAME: Configuracion_Registros
*----------------------------------------------------------------------------
* PARAMS:
* none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to configure init records
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void Configuracion_Registros (void){
    ADCON1bits.PCFG=0b1111;           // Disable input analog puerts A and B
    TRISA&=~(1<<0);                   // Config RA0 -> OUT digital.
    TRISA|=(1<<1);                    // Config RA1 -> IN digital.
}
/*F**************************************************************************
* NAME: Timer1_Init
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to configure the Timer 1. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void Timer1_Init (void){
    T1CONbits.RD16=1;                 // Timer1 --> 16 bits.
    T1CONbits.T1CKPS=0b00;            // Timer1  Pre-escaler=0.
    T1CONbits.TMR1CS=0;               // Internal clock (Fosc/4).
    TMR1=0;                           // TMR1 --> 0.
    TMR1ON=0;                         // Stop Timer1.
}
/*F**************************************************************************
* NAME: Obtener_Distancia
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Method to get the distance when using the Ultrasonico sensor. 
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
uint16_t Obtener_Distancia (void)
{
    uint16_t Duracion;         
    uint16_t Distancia;
    uint16_t Timer_1;
    
    Pin_Trig=1;                     
    __delay_us(10);                     // delay -> 10 us.
    Pin_Trig = 0;                       
    while(Pin_Echo == 0);               // Wait RA1 --> 0.
    T1CONbits.TMR1ON = 1;               // Enable Timer1.
    while(Pin_Echo == 1);               // Wait RA1 --> 1.
    T1CONbits.TMR1ON = 0;               // Stop Timer1.
    Timer_1 = TMR1;                     // "Timer_1" with the value TMR1. (time in us)
    Duracion = Timer_1/2;               // Charge the value the variable "Timer_1"/2 (debido que con 8MHz se generan 0.5us) en la variable "Duracion".
    if(Duracion <= 23200)               // 23529.4 us --> a 400cm.
    {
        Distancia = Duracion/58;        // Value distance in cm. (formula aplicada para us)
    }
    else if(Duracion < 116)             // 117.6 us --> 2 cm.
    {
        Distancia=0;                    // Value distance in cm.
    }
    else
    {
        Distancia=0;             
    }
    Duracion=0;                        // Restart  value of variable "Duracion".
    TMR1=0;                            // Restar value of record TMR1.
    
    return Distancia;                 // Return  value distance.
}