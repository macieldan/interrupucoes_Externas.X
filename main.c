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

void led_azul();
void led_vermelho(); 

//tratador de interrupções 

void __interrupt(high_priority) inteRupZERO();
void __interrupt(low_priority) inteRupUM();

void main(void) {
   
   //configura RD0 e RD1 como entrada de dados 
   //0 - output
   //1 - input
   
   TRISDbits.RD0 = 0;
   TRISDbits.RD1  = 0;
   
         //configuração das interrupções externas
   
   //INTCONbits.GIE = 1;           //habilita as interrupções  
   RCONbits.IPEN = 1;            //habilita os níveis de prioridade das interrupções
   INTCONbits.GIEH = 1;          //habilita prioridades de alto nível
   INTCONbits.GIEL = 1;          //habilita prioridades de baixo nível
   INTCON2bits.RBPU = 0;         //habilita os RPU ALL PORTB
   
      // interrupção zero 
   INTCONbits.INT0IE = 1;        //habilta a interupção zero
   INTCONbits.INT0IF = 0;        // flag da interrupção zero, o padão já é disable 
   INTCON2bits.INTEDG0 = 0;      //habilita a interrupção zero para a borda de descida. 
   
      //interrupção um 
   INTCON3bits.INT1IE = 1;       //habilta a interupção um   
   INTCON3bits.INT1IF = 0;       //flag da interrupção um, o padão já é disable
   INTCON2bits.INTEDG1 = 0;      //habilita a interrupção um para a borda de descida.
   INTCON3bits.INT1IP = 0;       //Ajusta a interrupção para baixa prioridade, o padrão é high
   
    
    
   

   return;
}

void __interrupt(high_priority) inteRupZERO(){
   
   if (INTCONbits.INT0IF){
      led_azul();
      __delay_ms(2000);                //enquanto a interrupção UM (HIGH PRIORITY) esta em execução, as demais inerrupções LP param de executar 
      INTCONbits.INT0IF = 0;
   }
}
void __interrupt(low_priority) inteRupUM(){
   
   if(INTCON3bits.INT1IF){
      led_vermelho();
      INTCON3bits.INT1IF = 0;
   }
}


void led_azul(){
   PORTDbits.RD0 = ~PORTDbits.RD0;
}

void led_vermelho(){
   PORTDbits.RD1 = ~PORTDbits.RD1;
}
