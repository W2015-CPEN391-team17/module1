#ifndef GPS_H_
#define GPS_H_

//Control registers
#define GPS_CONTROL (*(volatile unsigned char *)(0x84000210))
#define GPS_STATUS  (*(volatile unsigned char *)(0x84000210))
#define GPS_TXDATA  (*(volatile unsigned char *)(0x84000212))
#define GPS_RXDATA  (*(volatile unsigned char *)(0x84000212))
#define GPS_BAUD 	(*(volatile unsigned char *)(0x84000214))

#define hex0_1		(*(volatile unsigned char *)(0x80001110))
#define hex2_3		(*(volatile unsigned char *)(0x80001100))
#define hex4_5		(*(volatile unsigned char *)(0x800010b0))
#define hex6_7		(*(volatile unsigned char *)(0x800010a0))
#define switches	(volatile int *) 0x80001050

#define TIME_DATA 1
#define LAT_DATA 2
#define NS_DATA 3
#define LONG_DATA 4
#define EW_DATA 5
#define POS_DATA 6
#define SAT_DATA 7
#define HDOP_DATA 8
#define ALT_DATA 9
#define ALT_UNITS_DATA 10
#define GEO_DATA 11
#define GEO_UNITS_DATA 12

#define GPS_STATUS_TX_MASK 0x02
#define GPS_STATUS_RX_MASK 0x01

void init_gps(void);
char putchar_gps(char c);
char getchar_gps(void);
void send_command(void);

void read_gps_lcd(void);
void lcd_time(alt_up_character_lcd_dev * char_lcd_dev, char *time);
void lcd_latitude(alt_up_character_lcd_dev * char_lcd_dev, char *latitude, char *NS);
void lcd_longitude(alt_up_character_lcd_dev * char_lcd_dev, char *longitude, char *EW);
void lcd_position(alt_up_character_lcd_dev * char_lcd_dev, char *position);
void lcd_satellites(alt_up_character_lcd_dev * char_lcd_dev, char *num_satellites);
void lcd_hdop(alt_up_character_lcd_dev * char_lcd_dev, char *HDOP);
void lcd_altitude(alt_up_character_lcd_dev * char_lcd_dev, char *altitude, char *alt_units);
void lcd_geo(alt_up_character_lcd_dev * char_lcd_dev, char *geoidal, char *geo_units);

#endif /* GPS_H_ */
