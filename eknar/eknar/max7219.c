/*
 * max7219.c
 *
 * Created: 10.01.2023 18:21:19
 *  Author: Komputer_4
 */ 

#include <avr/io.h>

char ruch = 0b01000000;
char ekran[8];



void spi_send(int address, int data)
{
	//wysylanie danych przez spi
	PORTB	&=~	(1	<<	PB2); //rozpoczecie transmisji, SS na niski
	SPDR	= address;
	while(!(SPSR & (1 << SPIF))); //czekamy na zakonczenie wysylania
	SPDR	= data;
	while(!(SPSR & (1 << SPIF))); //czekamy na zakonczenie wysylania
	PORTB	|=	(1	<<	PB2); //zakonczenie transmisji, SS na wysoki
}

void spi_send_display()
{
	for(int i = 0; i<8; i++)	spi_send(i+1, ekran[i]);
}

void frame()
{
	ekran[0] = 0b11111111;
	for(int i = 1; i<7; i++)	ekran[i] |=  0b10000001;
	ekran[7] = 0b11111111;
}

void spi_init(void)
{
	//ustawiamy SS na wyjœcie
	DDRB	|=	(1	<<	PB2);
	PORTB	|=	(1	<<	PB2);
	//pin MOSI na wyjœcie
	DDRB	|=	(1	<<	PB3);
	//pin SCK na wyjœcie
	DDRB	|=	(1	<<	PB5);
	//wlaczamy masterka
	SPCR	|=	(1	<<	MSTR);
	//preskaler
	SPCR	&=~	(1	<<	SPR0);
	SPCR	&=~	(1	<<	SPR1);
	SPSR	|=	(1	<<	SPI2X);
	//SPI w³¹cz
	SPCR	|=	(1	<<	SPE);
}

void dot_dot(char step)
{
	frame();
	spi_send_display();
}
