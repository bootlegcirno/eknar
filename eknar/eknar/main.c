/*
 * eknar.c
 *
 * Created: 09.12.2022 19:33:15
 * Author : macko
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "max7219.h"


int main(void)
{
	
	spi_init();
	
	spi_send(SHUTDOWN_REG, NORMAL_OPERATION_S);	
	spi_send(DISPLAY_TEST_REG, NORMAL_OPERATION_DT);
	spi_send(SCAN_LIMIT_REG, 7);
	spi_send(0x01, 255);
	spi_send(0x02, 129);
	spi_send(0x03, 129);
	spi_send(0x04, 129);
	spi_send(0x05, 129);
	spi_send(0x06, 129);
	spi_send(0x07, 129);
	spi_send(0x08, 255);

	
	
	#define PRAWO 0
	#define LEWO 1
	char kierunek = PRAWO;
	
    while (1) 
    {
		dot_dot(1);
		//spi_send(0x05, ruch | 0b10000001);
		
		if(kierunek == PRAWO) 
		{
			ruch >>= 1;
			if(ruch == 0b00000010) kierunek = LEWO;
		}
		else 
		{
			ruch	<<= 1;
			if(ruch == 0b01000000) kierunek = PRAWO;
		}
		
		
		_delay_ms(200);
    }
}

