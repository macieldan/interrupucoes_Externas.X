/*
 * File:   main.c
 * Author: Daniel Maciel 
 * Matricula: 21150197
 *
 * Created on 21 de Junho de 2021, 07:13
 * 
 * Programa para exemplificar uso das interrup��es externas no PIC18F4520, 
 * s�o duas interrup��es, a interrup��o Zero possui prioridade alta, 
 * e a interrup��o UM prioridade baixa. A interrup��o de baixa prioridade 
 * n�o pode executar enquanto a de alta esta em execu��o. 
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



//tratadores de interrup��es 
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
      __delay_ms(300);     //evitar mal contato do bot�o
            /* 
             * Enquanto a interrup��o UM (HIGH PRIORITY) esta em execu��o, 
             * as demais interrup��es Low Priority n�o podem ser chamadas
             * caso a porta RB0 emita outro sinal low, a interrup��o termina sua execu��o e � liberada. 
             * Somente assim outras interrup��es podem ser chamadas, ou finalizar suas execu��es
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
         display_UM();                    //o display fica piscando apenas para indicar que a interrup��o esta em execu��o            
         if(!PORTBbits.RB1) break;
      }
      INTCON3bits.INT1IF = 0;
   }
}



void main(void) {
   
   //configura RD0 e RD1 como sa�da de dados 
   //0 - output
   //1 - input
   
   TRISDbits.RD0 = 0;
   TRISDbits.RD1  = 0;
   
   //TRISBbits.RB0 = 1; 
   //TRISBbits.RB1 = 1;
   
         //configura��o das interrup��es externas
   
      //INTCONbits.GIE = 1;           //habilita as interrup��es  
   RCONbits.IPEN = 1;            //habilita os n�veis de prioridade das interrup��es
   INTCONbits.GIEH = 1;          //habilita prioridades de alto n�vel
   INTCONbits.GIEL = 1;          //habilita prioridades de baixo n�vel
   INTCON2bits.RBPU = 0;         //habilita os RPU ALL PORTB
   
      // interrup��o zero 
   INTCONbits.INT0IE = 1;        //habilta a interup��o zero
   INTCONbits.INT0IF = 0;        // flag da interrup��o zero, o pad�o j� � disable 
   INTCON2bits.INTEDG0 = 0;      //habilita a interrup��o zero para a borda de descida. 
   
      //interrup��o um 
   INTCON3bits.INT1IE = 1;       //habilta a interup��o um   
   INTCON3bits.INT1IF = 0;       //flag da interrup��o um, o pad�o j� � disable
   INTCON2bits.INTEDG1 = 0;      //habilita a interrup��o um para a borda de descida.
   INTCON3bits.INT1IP = 0;       //Ajusta a interrup��o para baixa prioridade, o padr�o � high
   
   return;
}

