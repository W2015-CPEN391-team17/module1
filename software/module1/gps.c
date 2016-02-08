/*
 * Functions to read/write bytes to and from the GPS
 * Functions to read data from GPS, parse it, and display on the LCD. This
 * is done using the GPGGA format (real time).
 *
 */

#include <stdio.h>
#include <string.h>
#include "altera_up_avalon_character_lcd.h"
#include "gps.h"

//call this function at the start of the program before
//attempting to read or write
void init_gps(void)
{
	//set up 6850 control register to utilize a divide by 16 clock,
	//set RTS low, use 8 bits of data, no parity, 1 stop bit
	//transmitter interrupt disabled
	//program baud rate generator to use 9.6k baud
	GPS_CONTROL = 0x15;
	GPS_BAUD = 0x5;
}

// putchar_gps allows us to write 1 byte of data from the serial port.
char putchar_gps(char c)
{
	//poll Tx bit in 6850 status register. Wait for it to become '1'
	//write 'c' to the 6850 TxData register to output the character
	while(1)
	{
		if (GPS_STATUS & GPS_STATUS_TX_MASK)
		{
			GPS_TXDATA = c;
			return c;
		}
	}

	return c;
}

// getchar_gps allows us to read 1 byte of data from the serial port.
char getchar_gps(void)
{
	//poll Rx bit in 6850 status register. Wait for it to become '1'
	//read received character from 6850 register

	while(1)
	{
		if (GPS_STATUS & GPS_STATUS_RX_MASK)
		{
			return GPS_RXDATA;
		}
	}
	return "e";
}

// test function to send a command via serial port, and begin printing
// output characters in the console.
void send_command(void){

	int i;
	char out_char;
	const char start_log[] = "$PMTK622,1*29\r\n";
	int length = strlen(start_log);

	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(start_log[i]);
	}

	while(1){
		out_char = getchar_gps();
		printf("%c", out_char);
	}
}

// test function to read real-time gps data, parse it, and display
// it on the LCD. Switches used to toggle type of data displayed.
void read_gps_lcd(void){
	alt_up_character_lcd_dev *char_lcd_dev;
	char_lcd_dev = alt_up_character_lcd_open_dev("/dev/character_lcd_0");
	if( char_lcd_dev == NULL )
		alt_printf("Error: Could not open character LCD device\n");
	else
		alt_printf("Opened character LCD device\n");
	alt_up_character_lcd_init(char_lcd_dev);

	int i;
	char out_char;
	const char start_log[] = "$PMTK622,1*29\r\n";
	int length = strlen(start_log);

	int count = 0;
	int header = 0;
	int done = 0;
	int next = 0;
	char output = 0;
	char ID[10] = {0};
	char time[15] = {0};
	char latitude[15] = {0};
	char NS[2] = {0};
	char longitude[15] = {0};
	char EW[2] = {0};
	char position[2] = {0};
	char num_satellites[3] = {0};
	char HDOP[10] = {0};
	char altitude[10] = {0};
	char alt_units[2] = {0};
	char geoidal[10] = {0};
	char geo_units[2] = {0};

	int switch_val;

	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(start_log[i]);
	}

	while( done == 0 ){
		if( header != 1 ){

			output = getchar_gps();

			// check that the character sequence matches GPGGA
			if( output == '$'){
				output = getchar_gps();
				printf("%c", output);
				if( output == 'G'){
					output = getchar_gps();
					printf("%c", output);
					if( output == 'P'){
						output = getchar_gps();
						printf("%c", output);
						if( output == 'G'){
							output = getchar_gps();
							printf("%c", output);
							if( output == 'G'){
								output = getchar_gps();
								printf("%c\n", output);
								if( output == 'A'){
									header = 1;
								}
							}
						}
					}
				}
			}
		}
		else {

			output = getchar_gps();

			if(output == '*'){
				done = 1;
				printf("Time: %s\n", time);
				printf("Latitude: %s\n", latitude);
				printf("NS: %s\n", NS);
				printf("Longitude: %s\n", longitude);
				printf("EW: %s\n", EW);
				printf("Position: %s\n", position);
				printf("Satellites: %s\n", num_satellites);
				printf("HDOP: %s\n", HDOP);
				printf("Altitude: %s\n", altitude);
				printf("Alt Units: %s\n", alt_units);
				printf("Geoidal: %s\n", geoidal);
				printf("Geo Units: %s\n", geo_units);
			}
			else if(output == ','){
				next++;
				i = 0;
			}
			else {
				if(next == TIME_DATA){
					time[i] = output;
				}
				else if(next == LAT_DATA){
					latitude[i] = output;
				}
				else if(next == NS_DATA){
					NS[i] = output;
				}
				else if(next == LONG_DATA){
					longitude[i] = output;
				}
				else if(next == EW_DATA){
					EW[i] = output;
				}
				else if(next == POS_DATA){
					position[i] = output;
				}
				else if(next == SAT_DATA){
					num_satellites[i] = output;
				}
				else if(next == HDOP_DATA){
					HDOP[i] = output;
				}
				else if(next == ALT_DATA){
					altitude[i] = output;
				}
				else if(next == ALT_UNITS_DATA){
					alt_units[i] = output;
				}
				else if(next == GEO_DATA){
					geoidal[i] = output;
				}
				else if(next == GEO_UNITS_DATA){
					geo_units[i] = output;
				}
				i++;
			}
		}
	}

	while(1){
		switch_val = *(switches);
		if(switch_val == TIME_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_time(char_lcd_dev, time);
		}
		else if(switch_val == LAT_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_latitude(char_lcd_dev, latitude, NS);
		}
		else if(switch_val == LONG_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_longitude(char_lcd_dev, longitude, EW);
		}
		else if(switch_val == POS_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_position(char_lcd_dev, position);
		}
		else if(switch_val == SAT_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_satellites(char_lcd_dev, num_satellites);
		}
		else if(switch_val == HDOP_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_hdop(char_lcd_dev, HDOP);
		}
		else if(switch_val == ALT_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_altitude(char_lcd_dev, altitude, alt_units);
		}
		else if(switch_val == GEO_DATA){
			alt_up_character_lcd_init(char_lcd_dev);
			lcd_geo(char_lcd_dev, geoidal, geo_units);
		}
	}
}

void lcd_time(alt_up_character_lcd_dev * char_lcd_dev, char *time){
	alt_up_character_lcd_string(char_lcd_dev,"Time: ");
	alt_up_character_lcd_string(char_lcd_dev, time);
}

void lcd_latitude(alt_up_character_lcd_dev * char_lcd_dev, char *latitude, char *NS){
	alt_up_character_lcd_string(char_lcd_dev,"Lat: ");
	alt_up_character_lcd_string(char_lcd_dev, latitude);
	alt_up_character_lcd_string(char_lcd_dev, NS);
}

void lcd_longitude(alt_up_character_lcd_dev * char_lcd_dev, char *longitude, char *EW){
	alt_up_character_lcd_string(char_lcd_dev,"Lon: ");
	alt_up_character_lcd_string(char_lcd_dev, longitude);
	alt_up_character_lcd_string(char_lcd_dev, EW);
}

void lcd_position(alt_up_character_lcd_dev * char_lcd_dev, char *position){
	alt_up_character_lcd_string(char_lcd_dev,"Position: ");
	alt_up_character_lcd_string(char_lcd_dev, position);
}

void lcd_satellites(alt_up_character_lcd_dev * char_lcd_dev, char *num_satellites){
	alt_up_character_lcd_string(char_lcd_dev,"Satellites: ");
	alt_up_character_lcd_string(char_lcd_dev, num_satellites);
}

void lcd_hdop(alt_up_character_lcd_dev * char_lcd_dev, char *HDOP){
	alt_up_character_lcd_string(char_lcd_dev,"HDOP: ");
	alt_up_character_lcd_string(char_lcd_dev, HDOP);
}

void lcd_altitude(alt_up_character_lcd_dev * char_lcd_dev, char *altitude, char *alt_units){
	alt_up_character_lcd_string(char_lcd_dev,"Alt: ");
	alt_up_character_lcd_string(char_lcd_dev, altitude);
	alt_up_character_lcd_string(char_lcd_dev, alt_units);
}

void lcd_geo(alt_up_character_lcd_dev * char_lcd_dev, char *geoidal, char *geo_units){
	alt_up_character_lcd_string(char_lcd_dev,"Geo: ");
	alt_up_character_lcd_string(char_lcd_dev, geoidal);
	alt_up_character_lcd_string(char_lcd_dev, geo_units);
}





