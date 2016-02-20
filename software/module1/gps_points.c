/*
 * GPS Data Logger
 * This data logger start the log - after there is data in the log, we write it
 * to an array of structs containing latitude and longitude coordinates in
 * order to map them to the screen later.
 *
 */

#include <stdio.h>
#include <string.h>
#include "gps_points.h"
#include "graphics.h"

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

int swapEndian(char *s){
	register int val;

	val = strtoul(s, NULL, 16);
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
	val = (val << 16) | ((val >> 16) & 0xFFFF);

	return val;
}

float FloatToLatitudeConversion(int x){

	float *ptr = (float *)(&x); // cast int to float
	float f = *ptr; // get the float

	return f;
}

float FloatToLongitudeConversion(int x){

	float *ptr = (float *)(&x);
	float f = *ptr;

	return f;
}

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

// saves points as type float to an array of structs.
void save_points(void){

	printf("starting dump\n");
	int i, lat_count, lat_end, long_count, long_end, save_count;
	int j = 200;
	int k = 0;
	int n = 23;
	int s = 0;
	int colour = 1;
	int x = XRES/4;
	int y = YRES/2;
	int r = XRES/8;
	int count, log_count = 0;
	int place = 0;
	const char command[] = "$PMTK622,1*29\r\n";
	int length = strlen(command);
	char cur_string[256] = {0};

	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}

	while(gps_log[0].string[7] != 'X'){
		read_string(gps_log[0].string);
	}

	read_string(gps_log[0].string); // to dump the first unused log

	while(gps_log[0].string[11] == '0'){
		read_string(gps_log[0].string);
	}

	for(log_count = 1; log_count < 20; log_count++){
		read_string(gps_log[log_count].string);
		j = (n + j + 1) % 500;
		n = (n + k*j) % 350;
		k = (k + j + n + r) % 50;
		colour = (colour + n + j + k + r + s) % 8;
		x += (XRES-(XRES/2))/20;
		WriteCircle(x, y, r, colour);
	}

	for(log_count = 0; log_count < 9; log_count++){

		strcpy(cur_string, (const char *)gps_log[log_count].string);

		lat_count = 24;
		long_count = 33;
		lat_end = 32;
		long_end = 41;

		for(count = 0; count < 6; count++){
			save_count = 0;
			while(lat_count <= lat_end){
				if(cur_string[lat_count] != ','){
					gps_points[place].latitude[save_count] = cur_string[lat_count];
					lat_count++;
					save_count++;
				}
				else
					lat_count++;
			}
			save_count = 0;
			while(long_count <= long_end){
				if(cur_string[long_count] != ','){
					gps_points[place].longitude[save_count] = cur_string[long_count];
					long_count++;
					save_count++;
				}
				else
					long_count++;
			}
			gps_points[place].long_swapped = swapEndian(gps_points[place].longitude);
			gps_points[place].lat_swapped = swapEndian(gps_points[place].latitude);

			gps_points[place].long_float = FloatToLongitudeConversion(gps_points[place].long_swapped);
			gps_points[place].lat_float = FloatToLatitudeConversion(gps_points[place].lat_swapped);

			place++;
			lat_count += 27;
			long_count += 27;
			lat_end += 36;
			long_end += 36;
		}
	}

	for(log_count = 9; log_count < 12; log_count++){

		strcpy(cur_string, (const char *)gps_log[log_count].string);

		lat_count = 25;
		long_count = 34;
		lat_end = 33;
		long_end = 42;

		for(count = 0; count < 6; count++){
			save_count = 0;
			while(lat_count <= lat_end){
				if(cur_string[lat_count] != ','){
					gps_points[place].latitude[save_count] = cur_string[lat_count];
					lat_count++;
					save_count++;
				}
				else
					lat_count++;
			}
			save_count = 0;
			while(long_count <= long_end){
				if(cur_string[long_count] != ','){
					gps_points[place].longitude[save_count] = cur_string[long_count];
					long_count++;
					save_count++;
				}
				else
					long_count++;
			}
			gps_points[place].long_swapped = swapEndian(gps_points[place].longitude);
			gps_points[place].lat_swapped = swapEndian(gps_points[place].latitude);

			gps_points[place].long_float = FloatToLongitudeConversion(gps_points[place].long_swapped);
			gps_points[place].lat_float = FloatToLatitudeConversion(gps_points[place].lat_swapped);

			place++;
			lat_count += 27;
			long_count += 27;
			lat_end += 36;
			long_end += 36;
		}
	}

	return;

}

// log_now will log a point at that instant. Ensure you sleep 3 seconds before
// calling log_now another time.
void log_now(void){
	int i;
	const char command[] = "$PMTK186,1*20\r\n";
	int length = strlen(command);
	char string[256] = {0};

	printf("Logging...\n");
	// here we send the command to the gps
	for(i = 0; i < length; i++){
		putchar_gps(command[i]);
	}
	while(string[4] != 'K'){
		read_string(string);
	}
	printf("%s\n", string);
}

// starts the log.
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

// stops the log.
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

// erases the log.
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

void save_demo_points(void) {
	float latitude = 49.266500;
	float longitude = -123.248000;
	int i = 0;

	for( i = 0; i < 49; i++ ){
		latitude += 0.000020;
		longitude -= 0.000020;

		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}
	for( i = 49; i < 99; i++ ){
		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}

/*
	for( i = 9; i < 19; i++ ){
		latitude -= 0.000050;
		longitude -= 0.000030;

		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}

	for( i = 19; i < 29; i++ ){
		longitude -= 0.000030;

		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}

	for( i = 29; i < 49; i++ ){
		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}

	for( i = 49; i < 69; i++ ){
		latitude -= 0.000040;
		longitude -= 0.000050;

		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}

	for( i = 69; i < 79; i++ ){
		longitude -= 0.000030;

		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}

	for( i = 79; i < 99; i++ ){
		longitude -= 0.000060;

		gps_points[i].lat_float = latitude;
		gps_points[i].long_float = longitude;
	}
*/
	for( i = 0; i < 99; i++ ) {
		printf("latitude %d: %f  longitude %d: %f\n", i, gps_points[i].lat_float,
													  i, gps_points[i].long_float);
	}
}
