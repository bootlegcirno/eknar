/*
 * max7219.c
 *
 * Created: 10.01.2023 18:21:19
 *  Author: Komputer_4
 */ 


/*
Jak wygl¹daj¹ koordynaty:
  7 6 5 4 3 2 1 0
0 1 1 1 1 1 1 1 1 
1 1 0 0 0 0 0 0 1
2 1 0 0 0 X 0 0 1
3 1 0 0 0 0 0 0 1
4 1 0 0 0 0 0 0 1
5 1 0 0 0 0 0 0 1
6 1 0 0 0 0 0 0 1
7 1 1 1 1 1 1 1 1
*/

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "max7219.h"

//char ruch = 0b01000000;
char ekran[8];
//char kierunek;
//int step = 0;
char ball_x = 1;
char ball_y = 1;
float nuty_hz[] = { 16.35,  17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87};
char oktawa[] = "cCdDefFgGaAHP";

char melodia[] =			"DDDAPG"
char melodia_oktawy[] =		"445404";
char melodia_dlugosc[] =	"112211"
signed char ball_krok_x = PRAWO;
signed char ball_krok_y = DOL;




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

void spi_send_display(void)
{
	for(int i = 0; i<8; i++)	spi_send(i+1, ekran[i]);
}

void frame(void)
{
	update();
	ekran[0] =	0b11111111;
	for(int i = 1; i<7; i++)	ekran[i] |=  0b10000001;
	ekran[7] =	0b11111111;
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

void timer1_init(void)
{
	//set FASTPWM ICR1 8-bit on timer1 (timer2 occupies OC2/MOSI pin used by spi_init() )
	TCCR1A	|=	(1	<<	WGM11);
	TCCR1B	|=	(1	<<	WGM12);
	TCCR1B	|=	(1	<<	WGM13);
	//prescaler
	TCCR1B	|=	(1	<<	CS11);
	// OC1A pin onto exit (PB1)
	DDRB	|=	(1	<<	PB1);
	// set OC1
	
	ICR1  = 450;
	OCR1A = ICR1/2;
	// FASTPWM non-inverting
	TCCR1A	|=	(1	<<	COM1A1);
}



void dot_dot(void)
{
	cls();

	
	
	/*
  7 6 5 4 3 2 1 0
0 1 1 1 1 1 1 1 1 
1 1 0 0 0 0 0 0 1
2 1 0 0 0 0 0 0 1
3 1 0 0 Y 0 0 0 1
4 1 X 0 0 0 0 0 1
5 1 0 0 0 0 0 0 1
6 1 0 0 0 0 0 0 1
7 1 1 1 1 1 1 1 1
*/
	/*
	//ruch pi³ki oœ X
	if(ball_krok_x > 0 && ball_x > 5)		ball_krok_x = LEWO;  //granica lewa
	else if(ball_krok_x < 0 && ball_x < 2 )	ball_krok_x = PRAWO;	//granica prawa
	ball_x += ball_krok_x;

	//ruch pi³ki oœ Y
	if(ball_krok_y > 0 && ball_y > 5)		ball_krok_x = GORA;  //granica lewa
	else if(ball_krok_y < 0 && ball_y < 2 )	ball_krok_x = DOL;	//granica prawa
	ball_y += ball_krok_y;
	
	
	frame();
	
	// nanieœ pi³kê na  ekran
	update();

	spi_send_display();
	//cls();*/
}
	





void display_coords()
{
	printf("Ball's X coordinate: %d",ball_x);
	printf("Ball's Y coordinate: %d",ball_y);
}

void update()
{
	ekran[(int)ball_y] |= (1 << ball_x);
}

void cls(void)
{
	for(int i = 0; i<8; i++)
	{
		ekran[i] = 0b00000000;
	}
}



void dec2bin(uint8_t a)
{
	for(int8_t z = 0; z<8; z++)
	{
		if(a & (1<<z))  printf("1");
		else printf("0");
	}
}
void fullsend(void)
{
	
	frame();
	update();
	spi_send_display();
	cls();
}
void neuronactivation(void)
{

	for(int i = 0; i < 5; i++)
	{
		ball_y++;
		fullsend();
		_delay_ms(200);
	}
	for(int i = 0; i <5; i++)
	{
		ball_y--;
		ball_x++;
		fullsend();
		_delay_ms(200);
	}
	for(int i = 0; i < 5; i++)
	{
		ball_y++;
		fullsend();
		_delay_ms(200);
	}
	for(int i = 0; i <5; i++)
	{
		ball_y--;
		ball_x--;
		fullsend();
		_delay_ms(200);
	}
	frame();
	update();
	spi_send_display();
	cls();
}
// the microcontroller counts 125000 times a second
void soundtimer1(float hz)
{
	ICR1 = (int)(125000/hz);
	OCR1A = ICR1/2;
}

void music1()
{
	for(int i = 0; i < 3 ; i++)
	{
		soundtimer1(261.63);
		_delay_ms(200);
		soundtimer1(246.94);
		_delay_ms(200);
		soundtimer1(220.00);
		_delay_ms(200);
	}
	_delay_ms(600);
	soundtimer1(493.88);
	_delay_ms(400);
	soundtimer1(440.00);
	_delay_ms(600);
	soundtimer1(392.00);
	_delay_ms(400);
	soundtimer1(440.00);
	_delay_ms(600);
	soundtimer1(370);
	_delay_ms(400);
	soundtimer1(329.63);
	_delay_ms(800);
	
	
	
}
void screech()
{
	static int trolling = 1;
	soundtimer1(trolling++);
	_delay_ms(20);
}

void megalovania()
{
	
	soundtimer1(293.66);
	_delay_ms(125);
	soundtimer1(0);
	soundtimer1(293.66);
	_delay_ms(125);
	soundtimer1(587.33);
	_delay_ms(250);
	soundtimer1(440.00);
	_delay_ms(375);
	soundtimer1(415.30);
	_delay_ms(125);
	soundtimer1(392.00);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(250);
	soundtimer1( 293.66);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(125);
	soundtimer1( 392.00);
	_delay_ms(125);
	
	//to jest pierwszy takt dopiero teraz jest drugi
	
	soundtimer1( 261.63);
	_delay_ms(125);
	soundtimer1(0);
	soundtimer1( 261.63);
	_delay_ms(125);
	soundtimer1(587.33);
	_delay_ms(250);
	soundtimer1(440.00);
	_delay_ms(375);
	soundtimer1(415.30);
	_delay_ms(125);
	soundtimer1(392.00);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(250);
	soundtimer1( 293.66);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(125);
	soundtimer1( 392.00);
	_delay_ms(125);
	
	//drugi takt skonczony tera 3
	
	soundtimer1( 246.94);
	_delay_ms(125);
	soundtimer1(0);
	soundtimer1(  246.94);
	_delay_ms(125);
	soundtimer1(587.33);
	_delay_ms(250);
	soundtimer1(440.00);
	_delay_ms(375);
	soundtimer1(415.30);
	_delay_ms(125);
	soundtimer1(392.00);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(250);
	soundtimer1( 293.66);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(125);
	soundtimer1( 392.00);
	_delay_ms(125);

	// pora na 4
	
	soundtimer1( 233.08);
	_delay_ms(125);
	soundtimer1(0);
	soundtimer1( 233.08);
	_delay_ms(125);
	soundtimer1(587.33);
	_delay_ms(250);
	soundtimer1(440.00);
	_delay_ms(375);
	soundtimer1(415.30);
	_delay_ms(125);
	soundtimer1(392.00);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(250);
	soundtimer1( 293.66);
	_delay_ms(125);
	soundtimer1( 349.23);
	_delay_ms(125);
	soundtimer1( 392.00);
	_delay_ms(125);	
}