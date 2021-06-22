/*
 * File:   main.c
 * Author: Daniel
 *
 * Created on 21 de Junho de 2021, 07:13
 * 
 * Programa para exemplificar uso das interrupções externas no PIC18F4520
 * 
 */
       // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)


#include <xc.h>
#define _XTAL_FREQ 4000000

void led_zul();
void led_vermnelho(); 

void main(void) {
   
   //configura RBO e RDI como entrada de dados 
   //0 - output
   //1 - input
   
   TRISDbits.RD0 = 0;
   TRISDbits.RD0 = 0;
   
   while(1){
      if (PORTBbits.RB0){
         
      }
      if (PORTBbits.RB1){
         
      };
      __delay_ms(300);
   };
   
   return;
}

void led_zul(){
   PORTDbits.RD0 = ~PORTDbits.RD0;
}
void led_vermnelho(){
   PORTDbits.RD1 = ~PORTDbits.RD1;
}
