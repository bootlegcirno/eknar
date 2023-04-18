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

#define PRAWO	1
#define LEWO	-1
#define GORA	-1
#define DOL		1



extern void spi_init(void);
extern void spi_send(int address, int data);
extern void dot_dot(void);
extern void cls(void);
extern void frame(void);
extern void stp_reset(void);
extern void neuronactivation(void);
extern void spi_send_display(void);
extern void update(void);
extern void fullsend(void);

extern void timer1_init(void);
extern void soundtimer1(float hz);
extern void music1(void);


extern char ruch;
extern char kierunek;
extern int step;
extern char ball_x;
extern char ball_y;

#endif /* MAX7219_H_ */