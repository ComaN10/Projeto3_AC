#include <reg51.h>
 
#define tempoEspera 20 //contagem do timer0 para contar 1s(20*50 000us = 1s)

sbit greenLed1 = P1^0;
sbit redLed1 = P1^1;
sbit yellowLed = P1^2;
sbit greenLed2 = P1^3;
sbit redLed2 = P1^4;

unsigned char ciclos=0;//contador é incrementado a cada 50000us
unsigned char tempo=0; // contador é incrementado a cada 1s

unsigned int carsTotal = 9; //Numero de vagas no parque
unsigned char carro=0; //contador de entrada ou saida de carros
bit parkIn = 0;//S1
bit parkOut = 0; //S2

void Init(void);
void main(void){
    //Periphericals initialization
    greenLed1 = 1; //Liga a Luz Verde da entrada
    redLed1 = 0; //Desliga a Luz Vermelha da entrada
    greenLed2 = 1; //Liga a Luz Verde da saida 
    redLed2 = 0; //Desliga a Luz Vermelha da saida
    yellowLed=1; //Desliga a Luz amarela
    Init();
        while(1){ //loop infinito
            if(parkIn==1){
                greenLed1=0; //Led verde da entrada desliga
                greenLed2=1; //Led verde da saida ligado
                redLed1=1; //Led vermelho da entrada ligado
                redLed2=0; //Led vermelho da saida desligado
            }else if(parkOut==1){
                greenLed1=1;//Led verde da entrada ligado
                greenLed2=0;//Led verde da saida desligo
                redLed1=0;//Led vermelho da entrada desligado
                redLed2=1;//Led vermelho da saida ligado
            }else{ //caso esteja a entrar e a sair
                greenLed1=0;//Led verde da entrada desligado
                greenLed2=0;//Led verde da saida desligado
                redLed1=1;//Led vermelho da entrada ligado
                redLed2=1;//Led vermelho da saida ligado
            }
						if(ciclos>=tempoEspera){
            if((parkIn || parkOut) && tempo<5){
                yellowLed= ~yellowLed;
                tempo++;
								ciclos=0;
            }else{
                parkIn=0;
                parkOut=0;
                yellowLed=1;
                tempo=0;
            }
					}
            P2= carsTotal; 
        }  
}
void Init(void){
    EA =1; // Dá enable às interrupçoes
    ET0 =1; //Dá enable ao Timer
    EX0 =1; //Dá enable a interrupçoes externas
    EX1 =1; //Dá enable a interrupçoes externas
    //Configuracaio Registo TMOD
    TMOD &= 0x0F; //Limpa os 4 bits do timer 0
    TMOD |= 0xF1; //modo 1 do timer 0 (16 bits)
    //Configuracao TIMER 0
    TH0 = 0x3C;//Timer 0-50 000us
    TL0 = 0XB0;

    TR0 =1; //Comeca o timer 0
    //ativacao no falling edge~
    IT0 = 1;
    IT1 = 1;

}
void Timer0_ISR(void) interrupt 1{
    //Configuracao Timer 0
    TH0=0x3C; //Timer 0 -50 000us
    TL0=0xB0;
    ciclos++; //incrementa a cada contagem de 50 000us
}
void CarEntrance(void) interrupt 0{
    if(carsTotal>0 && parkIn==0 && parkOut==0){
        parkIn=1;
        carsTotal--;
    }
}
void CarLeave(void) interrupt 2{
     if(carsTotal<9 && parkOut==0 && parkIn==0){
        parkOut=1;
        carsTotal++;
    }
}
