/*
 * File:   main.c
 * Author: Daniel Maciel 
 * Matricula: 21150197
 *
 * Created on 21 de Junho de 2021, 07:13
 * 
 * Programa para exemplificar uso das interrupções externas no PIC18F4520, 
 * são duas interrupções, a interrupção Zero possui prioridade alta, 
 * e a interrupção UM prioridade baixa. A interrupção de baixa prioridade 
 * não pode executar enquanto a de alta esta em execução. 
 * 
 */

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)


#include <xc.h>
#define _XTAL_FREQ 4000000

void display_ZERO();
void display_UM(); 



//tratadores de interrupções 
void __interrupt(high_priority) inteRupZERO();
void __interrupt(low_priority) inteRupUM();

void display_ZERO(){
   PORTDbits.RD0 = ~PORTDbits.RD0;
}

void display_UM(){
   PORTDbits.RD1 = ~PORTDbits.RD1;
}

void __interrupt(high_priority) inteRupZERO(){

   if(INTCONbits.INT0IF){
      display_ZERO();
      __delay_ms(300);     //evitar mal contato do botão
            /* 
             * Enquanto a interrupção UM (HIGH PRIORITY) esta em execução, 
             * as demais interrupções Low Priority não podem ser chamadas
             * caso a porta RB0 emita outro sinal low, a interrupção termina sua execução e é liberada. 
             * Somente assim outras interrupções podem ser chamadas, ou finalizar suas execuções
             * Este while apenas simboliza um processo utilizando o recuso por um dado tempo
             */
      while(1){
         if(!PORTBbits.RB0) break;           
      }
      display_ZERO();
      INTCONbits.INT0IF = 0;
   }
}
void __interrupt(low_priority) inteRupUM(){
   
   if(INTCON3bits.INT1IF){
      display_UM();
      __delay_ms(500);
      while(1){
         display_UM();                    //o display fica piscando apenas para indicar que a interrupção esta em execução            
         if(!PORTBbits.RB1) break;
      }
      INTCON3bits.INT1IF = 0;
   }
}



void main(void) {
   
   //configura RD0 e RD1 como saída de dados 
   //0 - output
   //1 - input
   
   TRISDbits.RD0 = 0;
   TRISDbits.RD1  = 0;
   
   //TRISBbits.RB0 = 1; 
   //TRISBbits.RB1 = 1;
   
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

