/*
 * max7219.h
 *
 * Created: 10.01.2023 18:21:35
 *  Author: Komputer_4
 */ 


#ifndef MAX7219_H_
#define MAX7219_H_

#define SHUTDOWN_REG				0x0C
#define SHUTDOWN_MODE			0
#define NORMAL_OPERATION_S		1

#define INTENSITY_REG				0x0A

#define SCAN_LIMIT_REG				0x0B

#define DISPLAY_TEST_REG			0x0F
#define NORMAL_OPERATION_DT		0
#define DISPLAY_TEST_MODE		1

extern void spi_init(void);
extern void spi_send(int address, int data);
extern void dot_dot(int step);

extern char ruch;

#endif /* MAX7219_H_ */