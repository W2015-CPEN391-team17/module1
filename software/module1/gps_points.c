/*
 * GPS Data Logger
 * This data logger start the log - after there is data in the log, we write it
 * to an array of structs containing latitude and longitude coordinates in
 * order to map them to the screen later.
 *
 */

#include <stdio.h>
#include <string.h>
#include "altera_up_avalon_character_lcd.h"
#include "gps_points.h"
#include "gps.h"

struct points gps_points[10];

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
char getchar_gps(void){
	//poll Rx bit in 6850 status register. Wait for it to become '1'
	//read received character from 6850 register

	while(1)
	{
		if (GPS_STATUS & GPS_STATUS_RX_MASK)
		{
			return GPS_RXDATA;
		}
	}
	return '\0';
}

// swaps from little endian to big endian format
int swapEndian(char *s){
	register int val;

	val = strtoul(s, NULL, 16);
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
	val = (val << 16) | ((val >> 16) & 0xFFFF);

	return val;
}

// converts a 4 byte IEEE-754 format float (passed as an int representing longitude and latitude)
// to an ASCII string as a decimal representation of longitude/latitude
char *FloatToLatitudeConversion(int x){
	static char buff[100];

	float *ptr = (float *)(&x); // cast int to float
	float f = *ptr; // get the float

	sprintf(buff, "%2.4f", f); // write in string to an array

	return buff;
}

// converts a 4 byte IEEE-754 format float (passed as an int representing longitude and latitude)
// to an ASCII string as a decimal representation of longitude/latitude
char *FloatToLongitudeConversion(int x){
	static char buff[64];

	float *ptr = (float *)(&x);
	float f = *ptr;

	sprintf(buff, "%3.4f", f);
	return buff;
}

// reads one full string from the GPS between $ and an end line character.
void read_string(char *output){

//	printf("Reading string\n");
	memset(output, 0, 256);

	int i = 0;
	char c;
	int done = 0;

	while(done == 0){
		c = getchar_gps();

		if(c == '$'){
			while (c != '\n'){
				output[i] = c;
				c = getchar_gps();
				i++;
			}
			output[i] = '\0';
			//printf("%s\n", output);
			done = 1;
		}
	}
	return;
}

// configures log settings (i.e. interval on/off, rate of interval, etc.)
void config_log(void){
	int i;
	char out[20];
	const char command[] = "$PMTK187,1,5*35\r\n";
	int length = strlen(command);

	printf("Configuring to log every 5 seconds.\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}

	while(1){
		read_string(out);
		printf("%s", out);
	}
}

// reads data from the log, and parses the first useable log to obtain longitude and latitude values.
// values are saved into an array of structs to use as x,y coordinates for mapping
void save_points(void){

	printf("starting dump\n");
	int i, lat_count, lat_end, long_count, long_end, save_count;
	int count = 0;
	int place = 0;
	const char command[] = "$PMTK622,1*29\r\n";
	int length = strlen(command);
	char string[256] = {0};

	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}

	while(string[7] != 'X'){
		read_string(string);
		printf("string: %s\n", string);
	}

	lat_count = 24;
	long_count = 33;
	lat_end = 32;
	long_end = 41;

	read_string(string);
	printf("string2: %s\n", string);

	while(string[11] == '0'){
		read_string(string);
		printf("%s\n", string);
	}

	for(count = 0; count < 6; count++){
		save_count = 0;
		while(lat_count <= lat_end){
			if(string[lat_count] != ','){
				gps_points[place].latitude[save_count] = string[lat_count];
				lat_count++;
				save_count++;
			}
			else
				lat_count++;
		}
		save_count = 0;
		while(long_count <= long_end){
			if(string[long_count] != ','){
				gps_points[place].longitude[save_count] = string[long_count];
				long_count++;
				save_count++;
			}
			else
				long_count++;
		}
		gps_points[place].long_swapped = swapEndian(gps_points[place].longitude);
		gps_points[place].lat_swapped = swapEndian(gps_points[place].latitude);

		strcpy(gps_points[place].latitude, FloatToLatitudeConversion(gps_points[place].lat_swapped));
		strcpy(gps_points[place].longitude, FloatToLongitudeConversion(gps_points[place].long_swapped));

		printf("latitude %d: %s  longitude %d: %s\n", place, gps_points[place].latitude,
										          	  place, gps_points[place].longitude);

		place++;
		lat_count += 27;
		long_count += 27;
		lat_end += 36;
		long_end += 36;
	}

	return;
}

// immediately snapshots one log
void log_now(void){
	int i;
	const char command[] = "$PMTK186,1*20\r\n";
	int length = strlen(command);

	printf("Logging...\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}
}

// begin logging
void start_log(void){
	int i;
	const char command[] = "$PMTK185,0*22\r\n";
	int length = strlen(command);

	printf("Starting log...\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}
}

// end logging
void stop_log(void){
	int i;
	const char command[] = "$PMTK185,1*23\r\n";
	int length = strlen(command);

	printf("Stopping log...\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}
}

// erase the log
void erase_log(void){
	int i;
	const char command[] = "$PMTK184,1*22\r\n";
	int length = strlen(command);

	printf("Erasing log...\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}
}

// returns a string denoting settings of the log (i.e. interval, interval rate, etc.)
void query_log(void){
	int i;
	const char command[] = "$PMTK183*38\r\n";
	char out[100];
	//char search[] = "PMTK001,183,3*3A";
	int length = strlen(command);

	printf("Query...\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}

	read_string(out);
	while(out[0] != 'P' && out[15] != 'A'){
		read_string(out);
	}
	printf("%s", out);
}
