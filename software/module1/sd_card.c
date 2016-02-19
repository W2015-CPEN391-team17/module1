#include <stdio.h>
#include <stdlib.h>
#include <altera_up_sd_card_avalon_interface.h>
#include <string.h>
#include <assert.h>
#include "sd_card.h"
#include "conversion.h"

void sd_card_print_contents(char *filename)
{
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return;
	}
	else {
		printf("SDCard Open PASSED\n");
	}

	if (device_reference != NULL ) {
		while (1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					short int file_handle;
					if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
						file_handle = alt_up_sd_card_fopen(filename, false);
						if (file_handle == -1) {
							// File most likely does not exist, so attempt to create it
							file_handle = alt_up_sd_card_fopen(filename, true);
						}
						switch (file_handle) {
						case -1: {
							printf("File NOT Opened (file_handle is -1)\n");
							break;
						}
						case -2: {
							printf("File NOT Opened (file_handle is -2)\n");
							break;
						}
						default: {
							printf("File Opened\n");
							short int read = alt_up_sd_card_read(file_handle);
							while (!(read < 0)) {
								printf("%c", (char)read);
								read = alt_up_sd_card_read(file_handle);
							}
							printf("\n");
							printf("Closing file\n");
							if (alt_up_sd_card_fclose(file_handle) == false) {
								printf("WARNING: alt_up_sd_card_fclose was unsuccessful\n");
							}
							printf("Exiting sd_card_print_contents().\n");
							return;
						}
						}
					}
				}
				else {
					printf("Unknown file system.\n");
				}
				connected = 1;
				printf("Please disconnect the SD card.\n");
			}
			else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
				printf("Card disconnected.\n");
				printf("Exiting sd_card_print_contents().\n");
				return;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
}

#define BUF_SIZE 100	// This should be able to fit a single-precision float

void sd_card_write_point(GPSPoint point, char *filename) {
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return;
	}
	else {
		printf("SDCard Open PASSED\n");
	}

	if (device_reference != NULL ) {
		while (1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					short int file_handle;
					if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
						file_handle = alt_up_sd_card_fopen(filename, false);
						if (file_handle == -1) {
							// File most likely does not exist, so attempt to create it
							file_handle = alt_up_sd_card_fopen(filename, true);
						}
						switch (file_handle) {
						case -1: {
							printf("File NOT Opened (file_handle is -1)\n");
							break;
						}
						case -2: {
							printf("File NOT Opened (file_handle is -2)\n");
							break;
						}
						default: {
							printf("File Opened\n");

							// Convert GPSPoint floats to strings
							char x[BUF_SIZE] = "X";
							char y[BUF_SIZE] = "Y";
							snprintf(x, BUF_SIZE, "%f", point.x);
							snprintf(y, BUF_SIZE, "%f", point.y);

							// write to the SD card
							while (1) {
								int i;
								for (i = 0; x[i] != '\0'; i++) {
									if (alt_up_sd_card_write(file_handle, x[i]) == false) {
										printf("Error writing to file...\n");
										alt_up_sd_card_fclose(file_handle);
										return;
									}
								}
								if (alt_up_sd_card_write(file_handle, ',') == false) {
									printf("Error writing to file...\n");
									alt_up_sd_card_fclose(file_handle);
									return;
								}
								for (i = 0; y[i] != '\0'; i++) {
									if (alt_up_sd_card_write(file_handle, y[i]) == false) {
										printf("Error writing to file...\n");
										alt_up_sd_card_fclose(file_handle);
										return;
									}
								}
								if (alt_up_sd_card_write(file_handle, ';') == false) {
									printf("Error writing to file...\n");
									alt_up_sd_card_fclose(file_handle);
									return;
								}

								printf("Successfully wrote to the SD card\n");
								break;
							}
							if (alt_up_sd_card_fclose(file_handle) == false) {
								printf("WARNING: alt_up_sd_card_fclose was unsuccessful\n");
							}
							printf("Exiting sd_card_write_points()\n");
							return;
						}
						}
					}
				}
				else {
					printf("Unknown file system.\n");
				}
				connected = 1;
				printf("Please disconnect the SD card.\n");
			}
			else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
				printf("Card disconnected.\n");
				printf("Exiting sd_card_write_points()\n");
				return;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
}

#define WRITESTRLEN 2048

void sd_card_write_points(GPSPoint points[], unsigned long len, char *filename)
{
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return;
	}
	else {
		printf("SDCard Open PASSED\n");
	}

	if (device_reference != NULL ) {
		while (1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					short int file_handle;
					if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
						file_handle = alt_up_sd_card_fopen(filename, false);
						if (file_handle == -1) {
							// File most likely does not exist, so attempt to create it
							file_handle = alt_up_sd_card_fopen(filename, true);
						}
						switch (file_handle) {
						case -1: {
							printf("File NOT Opened (file_handle is -1)\n");
							break;
						}
						case -2: {
							printf("File NOT Opened (file_handle is -2)\n");
							break;
						}
						default: {
							printf("File Opened\n");

							// write to a write_str buffer
							char write_str[WRITESTRLEN] = "";
							int i;
							for (i = 0; i < len; i++) {
								char x_buf[BUF_SIZE];
								snprintf(x_buf, BUF_SIZE, "%f", points[i].x);
								strcat(write_str, x_buf);

								strcat(write_str, ",");

								char y_buf[BUF_SIZE];
								snprintf(y_buf, BUF_SIZE, "%f", points[i].y);
								strcat(write_str, y_buf);

								strcat(write_str, ";");
							}
							// write the buffer to the SD card
							for (i = 0; write_str[i] != '\0'; i++) {
								if (alt_up_sd_card_write(file_handle, write_str[i]) == false) {
									printf("Error writing to file...\n");
									alt_up_sd_card_fclose(file_handle);
									return;
								}
							}

							printf("Successfully wrote to the SD card\n");

							if (alt_up_sd_card_fclose(file_handle) == false) {
								printf("WARNING: alt_up_sd_card_fclose was unsuccessful\n");
							}
							printf("Exiting sd_card_write_points()\n");
							return;
						}
						}
					}
				}
				else {
					printf("Unknown file system.\n");
				}
				connected = 1;
				printf("Please disconnect the SD card.\n");
			}
			else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
				printf("Card disconnected.\n");
				printf("Exiting sd_card_write_points()\n");
				return;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
}

void sd_card_write_GPSPointSets(GPSPointSet sets[], unsigned long len, char *filename)
{
	assert(len < GPSPOINTSET_POINTSMAXLEN);

	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return;
	}
	else {
		printf("SDCard Open PASSED\n");
	}

	if (device_reference != NULL ) {
		while (1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					short int file_handle;
					if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
						file_handle = alt_up_sd_card_fopen(filename, false);
						if (file_handle == -1) {
							// File most likely does not exist, so attempt to create it
							file_handle = alt_up_sd_card_fopen(filename, true);
						}
						switch (file_handle) {
						case -1: {
							printf("File NOT Opened (file_handle is -1)\n");
							break;
						}
						case -2: {
							printf("File NOT Opened (file_handle is -2)\n");
							break;
						}
						default: {
							printf("File Opened\n");

							// write to a write_str buffer
							char write_str[WRITESTRLEN] = "";
							strcat(write_str, "{");
							int i;
							for (i = 0; i < len; i++) {
								strcat(write_str, "[");
								char temp_tag[BUF_SIZE];
								snprintf(temp_tag, BUF_SIZE, "%d", sets[i].tag);
								strcat(write_str, temp_tag);
								strcat(write_str, ":");
								int j;
								for (j = 0; j < sets[i].points_len; j++) {
									char x_buf[BUF_SIZE];
									snprintf(x_buf, BUF_SIZE, "%f", sets[i].points[j].x);
									strcat(write_str, x_buf);

									strcat(write_str, ",");

									char y_buf[BUF_SIZE];
									snprintf(y_buf, BUF_SIZE, "%f", sets[i].points[j].y);
									strcat(write_str, y_buf);

									strcat(write_str, ";");
								}
								strcat(write_str, "]");
							}
							strcat(write_str, "}");
							// write the buffer to the SD card
							for (i = 0; write_str[i] != '\0'; i++) {
								if (alt_up_sd_card_write(file_handle, write_str[i]) == false) {
									printf("Error writing to file...\n");
									alt_up_sd_card_fclose(file_handle);
									return;
								}
							}

							printf("Successfully wrote to the SD card\n");

							if (alt_up_sd_card_fclose(file_handle) == false) {
								printf("WARNING: alt_up_sd_card_fclose was unsuccessful\n");
							}
							printf("Exiting sd_card_write_points()\n");
							return;
						}
						}
					}
				}
				else {
					printf("Unknown file system.\n");
				}
				connected = 1;
				printf("Please disconnect the SD card.\n");
			}
			else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
				printf("Card disconnected.\n");
				printf("Exiting sd_card_write_points()\n");
				return;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
}
