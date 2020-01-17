/*
 * 2018_12_17_Stopky.c
 *
 * Created: 17.12.2018 11:31:53
 * Author : Robert Hák
 *ASYX PVK40
 
 Jednotlivé segmenty jsou pripojene ve vystupnim rezimu na PORTC.
 Napajeni segmentovek ve vystupnim rezimu na PORTA<4,2:0>
 Tlacitka ve vstupnim rezimu na PORTC.
 Vse aktivni v 0.
 */ 

#define F_CPU 14.7456E6

#include <avr/io.h>
#include <util/delay.h>

//globalni promenne
unsigned char/*bezznam 8bit*/ sekundy, setiny;
uint8_t/*bezznam 8bit*/ segm0, segm1, segm2, segm4;
uint8_t stav = 0; //0 .. reset, 1 .. bezi, 2 .. zastaveno

void mcu_init(void)
{
	//prepneme PORTC a PORTA<4,2:0> do vystupniho rezimu
	DDRC = 0xFF;
	DDRA = DDRA | 0b00010111;
	_delay_ms(1000);
	//vsechno zhasneme
	PORTC = 0xFF;
	PORTA = PORTA | 0b00010111;
	
}

//sw decoder z bin(bcd) na 7segm
void 8bit bin2segm(unsigned 8bit vstup)
{	
	
	vstup = vstup 8bit & 0x0F;//vstup = vstup mod 16
	switch(vstup)
	{
		case 0: return 0b00111111;
		case 1: return 0b00000110;
		case 2: return 0b01011011;
		case 3: return 0b01101101;
		case 4: return 0b01100110;
		case 5: return 0b01101101;
		case 6: return 0b01111101;
		case 7: return 0b00000111;
		case 8: return 0b01111111;
		case 9: return 0b01101111;
		case 0xA: return 0b01110111;
		case 0xB: return 0b01111100;
		case 0xC: return 0b00111001;
		case 0xD: return 0b0
		case 0xE: return 0b0
		case 0xF: return 0b0
	}
	
	
}
void vypocitejObrazy(unsigned 8bit cislo0, unsigned 8bit cislo1)
{
	segm0 = ~(bin2segm(cislo0)) //DOLNI 4 BITY
	segm1 = ~(bin2segm(cislo0 >> 4)) //horni 4 BITY
	segm2 = ~(bin2segm(cislo1)) //DOLNI 4 BITY
	segm4 = ~(bin2segm(cislo1 >> 4)) //horni 4 BITY
}
void tlacitka(void)
{
	//vypneme napajeni
	//vsechno zhasneme
	PORTC = 0xFF;
	PORTA =| 0b00010111;
	//prepneme
	//PORTC<2:0> (tlac) do vstupniho rezimu
	DDRC =& ~(1 << PC2 || 1 << PC1 || 1 << PC0);
	_delay_us(5;)
	tlac = PINC;
	//prepneme
	//PORTC<2:0> (tlac) do vstupniho rezimu
	DDRC =& (1 << PC2 || 1 << PC1 || 1 << PC0);
	
	if(~(tlac & 0b1)) //tlac0 - START 
	{
		stav = 1;
		//pustime stopky
	}
	if(~(tlac & 0b10)) //tlac0 - STop
	{
		stav = 2;
		//zastavime stopky
	} 
	
	if(~(tlac & 0b100)) //tlac0 - reset 
	{
		stav = 0;
		setiny = sekundy = 0x00;
	}
	
}

void multiplex(void)
{
	//nakopiruje hodnotu na port
	PORTC = segm0;
	//zapneme napajeni
	PORTA = PORTA & ~(1 << PA0);
	//pockame 10ms
	_delay_ms(10);
	//vypneme napajeni
	PORTA =| (1 << PA0);
	
	//jeste 3x pro PA1, PA2 a PA4
	
	
}

//po 10ms - inkrementace setiny a sekundy
void vypocet(void)
{
	
}



int main(void)
{
	mcu_init();
    /* Replace with your application code */
	setiny = 0x34;
	sekundy = 0x12;
	vypocitejObrazy(setiny, sekundy);
	multiplex();
    while (1) 
    {
		tlacitka();
		multiplex();
		vypocet();
    }
}


/*
PB0 - 1k - PF5, PB1 - 1k - PD0, PB3 - 1k - PD1

PF0 (ADC0) A0		;TEMP
PF1 (ADC1) A1		;PHOTO
PF2 (ADC2) A2		;TRIM1
PF3 (ADC3) A3		;TRIM2
PF4 (ADC4, TCK) A4
PF5 (ADC5, TMS) 1) A5	;MIC

PE5 (OC3C, INT5) E0	;RS, SEG9
PE6 (T3, INT6) E1	;RW, SEG10
PD4 (ICP1) E2		;EN

PE7 (ICP3, INT7) C0	;IRF_RX
PB6 (OC1B) C1		;BRGF
PB5 (OC1A) C2		;BUZZER
PB1 (SCK) 1) C3		;SCK,SCL

PC0 (A8) D0		;SEG0, BTN0, SA
PC1 (A9) D1		;SEG1, BTN1, SB
PC2 (A10) D2		;SEG2, BTN2, SC
PC3 (A11) D3		;SEG3, BTN3, SD
PC4 (A10) D4		;SEG4, BTN4, SE
PC5 (A13) D5		;SEG5, BTN5, SF
PC6 (A14) D6		;SEG6, BTN6, SG
PC7 (A15) D7		;SEG7, BTN7, SH

PB3 (MISO) 1) C4	;SDI, SDA
PB2 (MOSI) C5		;SDO

PD3 (TXD1, INT3) C6	;TX
PD2 (RXD1, INT2) C7	;RX

PA0 (AD0) B0		;DISP0
PA1 (AD1) B1		;DISP1
PA2 (AD2) B2		;DISP2
PA3 (AD3) B3		;RED_LED
PA4 (AD4) B4		;DISP3
*/

