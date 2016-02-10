#include <stdio.h>
#include <stdlib.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "sd_card.h"

int sd_card_write_points(void) {
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return 0;
	}
	else {
		printf("SDCard Open PASSED\n");
	}

	if (device_reference != NULL ) {
		if ((connected == 0) && (alt_up_sd_card_is_Present())){
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
				short int file_handle;
				if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
					if ((file_handle = alt_up_sd_card_fopen(filename, true)) != -1) {
						printf("File Opened\n");
						int i;
						for (i = 0; i < 1024; i++) {
							// TODO: save gps points to the SD card
							return 0;
						}
						printf("Done!!!\n");
						alt_up_sd_card_fclose(file_handle);
					}
					else {
						printf("File NOT Opened\n");
					}
				}
			}
			else {
				printf("Unknown file system.\n");
			}
			connected = 1;
		}
		else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
			printf("Card disconnected.\n");
			connected = 0;
		}
	}
	else {
		printf("Can't open device\n");
	}
	return 1;
}

void sd_card_test(void) {
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return;
	}
	else
		printf("SDCard Open PASSED\n");


	if (device_reference != NULL ) {
		while(1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					const char *filename = "text.txt";
					short int file_handle;
					if (alt_up_sd_card_is_Present() && alt_up_sd_card_is_FAT16()) {
						if ((file_handle = alt_up_sd_card_fopen(filename, true)) != -1) {
							printf("File Opened\n");
							int i;
							for (i = 0; i < 1024; i++) {
								if (alt_up_sd_card_write(file_handle, 'A') == false) {
									printf("Error writing to file...\n");
									return;
								}
							}
							printf("Done!!!\n");
							alt_up_sd_card_fclose(file_handle);
						}
						else {
							printf("File NOT Opened\n");
						}
					}
				}
				else {
					printf("Unknown file system.\n");
				}
				connected = 1;
			} else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
				printf("Card disconnected.\n");
				connected = 0;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
}
