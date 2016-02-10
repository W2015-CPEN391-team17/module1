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

struct points {
	char latitude[16];
	char longitude[16];
	int lat_swapped;
	int long_swapped;
};

void init_gps(void);
char putchar_gps(char c);
char getchar_gps(void);
int swapEndian(char *s);
char *FloatToLatitudeConversion(int x);
char *FloatToLongitudeConversion(int x);
void read_string(char *output);
void config_log(void);
void save_points(void);
void start_log(void);
void stop_log(void);
void erase_log(void);
void query_log(void);

#endif
