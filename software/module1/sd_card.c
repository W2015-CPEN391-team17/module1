#include <stdio.h>
#include <stdlib.h>
#include <altera_up_sd_card_avalon_interface.h>
#include <string.h>
#include "sd_card.h"
#include "datasets.h"
#include "cJSON.h"

#define JSON_DATA_MAX_LEN 2048

/////////////////////////
// Undocumented functions
void sd_card_cJSON_stringify(dataSet in[], char out[])
{
	// Convert GPSPoint and dataSet structs into JSON string data
	cJSON *root;
	root = cJSON_CreateArray();
	if (in != NULL) {
		int i;
		for (i = 0; i < MAX_N_SETS; i++) {
			// Convert dataSet into JSON string data
			cJSON *temp_dataset, *temp_gpspoints;
			cJSON_AddItemToArray(root, temp_dataset = cJSON_CreateObject());
			cJSON_AddNumberToObject(temp_dataset, "size", in[i].size);
			cJSON_AddItemToObject(temp_dataset, "gpspoints", temp_gpspoints = cJSON_CreateArray());
			// Convert gpspoints into JSON string data
			int j;
			for (j = 0; j < in[i].size; j++) {
				cJSON *temp_gpspoint;
				cJSON_AddItemToArray(temp_gpspoints, temp_gpspoint = cJSON_CreateObject());
				cJSON_AddItemToObject(temp_gpspoint, "x", cJSON_CreateNumber(in[i].points[j].x));
				cJSON_AddItemToObject(temp_gpspoint, "y", cJSON_CreateNumber(in[i].points[j].y));
			}
		}
	}

	// DEBUG
	printf("sd_card_cJSON_stringify: %s\n", cJSON_Print(root));

	strcat(out, cJSON_Print(root));

	// Cleanup cJSON data
	cJSON_Delete(root);
}

void sd_card_cJSON_parse(char data[], dataSet out[])
{
	// Convert JSON string data back into GPSPoint and dataSet structs

	int success = 0;  // set to 0 on successful conversion; otherwise set to 1
	dataSet new_set;  // dataSet retrieved from JSON string data

	cJSON *in_root = cJSON_Parse(data);
	int dataSet_index = 0;
	cJSON *in_dataSet = cJSON_GetArrayItem(in_root, dataSet_index);
	while (in_dataSet != NULL) {
		new_set.size = cJSON_GetObjectItem(in_dataSet, "size")->valuedouble;
		cJSON *gpspoints = cJSON_GetObjectItem(in_dataSet, "gpspoints");
		if (gpspoints != NULL) {
			// Read each gpspoint
			int gpspoint_index = 0;
			cJSON *gpspoint = cJSON_GetArrayItem(gpspoints, gpspoint_index);
			while (gpspoint != NULL) {

				// DEBUG
				printf("gpspoints_index: %d\n", gpspoint_index);

				GPSPoint new_pt;
				new_pt.x = cJSON_GetObjectItem(gpspoint, "x")->valuedouble;
				new_pt.y = cJSON_GetObjectItem(gpspoint, "y")->valuedouble;
				new_set.points[gpspoint_index] = new_pt;
				gpspoint_index++;
				gpspoint = cJSON_GetArrayItem(gpspoints, gpspoint_index);
			}
			cJSON_Delete(gpspoint);
		}
		else {
			success = 1;
		}

		// DEBUG
		printf("dataSet_index: %d\n", dataSet_index);

		out[dataSet_index] = new_set;
		cJSON_Delete(gpspoints);
		dataSet_index++;
		in_dataSet = cJSON_GetArrayItem(in_root, dataSet_index);
	}
	cJSON_Delete(in_dataSet);
	cJSON_Delete(in_root);
}

///////////////////////
// Documented functions
int sd_card_save(dataSet dsets[], char *filename)
{
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return -2;
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

							// Convert dsets[] into JSON string
							char json_data[JSON_DATA_MAX_LEN];
							sd_card_cJSON_stringify(dsets, json_data);

							// Write the buffer to the SD card
							int i;
							for (i = 0; json_data[i] != '\0'; i++) {
								if (alt_up_sd_card_write(file_handle, json_data[i]) == false) {
									printf("Error writing to file...\n");
									alt_up_sd_card_fclose(file_handle);
									return -1;
								}
							}

							printf("Successfully wrote to the SD card\n");

							if (alt_up_sd_card_fclose(file_handle) == false) {
								printf("WARNING: alt_up_sd_card_fclose was unsuccessful\n");
							}
							printf("Exiting sd_card_write_points()\n");
							return 0;
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
				return -3;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
	// Should not get to this point
	return 1;
}

int sd_card_load(dataSet out[], char *filename)
{
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return -2;
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
							char json_data[JSON_DATA_MAX_LEN];
							// Read data from the SD card into json_data
							short int read = alt_up_sd_card_read(file_handle);
							while (!(read < 0)) {
								char temp[1];
								temp[0] = (char)read;
								strcat(json_data, temp);
								read = alt_up_sd_card_read(file_handle);
							}

							// Generate dataset array from json_data
							// and assign it to out[]
							sd_card_cJSON_parse(json_data, out);

							printf("Closing file\n");
							if (alt_up_sd_card_fclose(file_handle) == false) {
								printf("WARNING: alt_up_sd_card_fclose was unsuccessful\n");
							}
							printf("Exiting sd_card_print_contents().\n");
							return 0;
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
				return -3;
			}
		}
	}
	else {
		printf("Can't open device\n");
	}
	// Should not get to this point
	return 1;
}
