#ifndef GPS_POINTS_H_
#define GPS_POINTS_H_

//Control registers
#define GPS_CONTROL (*(volatile unsigned char *)(0x84000210))
#define GPS_STATUS  (*(volatile unsigned char *)(0x84000210))
#define GPS_TXDATA  (*(volatile unsigned char *)(0x84000212))
#define GPS_RXDATA  (*(volatile unsigned char *)(0x84000212))
#define GPS_BAUD 	(*(volatile unsigned char *)(0x84000214))

#define GPS_STATUS_TX_MASK 0x02
#define GPS_STATUS_RX_MASK 0x01

#include "conversion.h"

struct points {
	//to parse from gps data
	char latitude[16];
	char longitude[16];

	// to save as float value
	float lat_float;
	float long_float;

	// to swap endianness
	int lat_swapped;
	int long_swapped;
};

struct logs {
	char string[256];
};

struct logs gps_log[50];
struct points gps_points[70];

void init_gps(void);
char putchar_gps(char c);
char getchar_gps(void);
int swapEndian(char *s);
float FloatToLatitudeConversion(int x);
float FloatToLongitudeConversion(int x);
void read_string(char *output);
void config_log(void);
void save_points(void);
void start_log(void);
void stop_log(void);
void erase_log(void);
void query_log(void);
void save_demo_points(int);

#endif // GPS_POINTS_H_
