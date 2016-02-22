#ifndef SD_CARD_H_
#define SD_CARD_H_

#include "datasets.h"
#include "cJSON.h"

// An array of dataSets are represented in JSON with this schema:
//[
// {
//	"size": <double>,
//	"gpspoints": [
//	              {
//					"x": <double>,
//					"y": <double>
//	              }
//	]
// }
//]

// Save dataSet dsets[] to the SD card.
// Return 0 on success, return -1 on error writing to the SD card,
// return -2 if the SD card could not be opened, return -3 if manual
// disconnection of the SD card was necessary, otherwise return a
// non-zero value.
int sd_card_save(dataSet dsets[], char *filename);

// Load dataSet from the SD card to dataSet out[].
// Return 0 on success, return -2 if SD card could not be opened,
// return -3 if manual disconnection of the SD card was necessary,
// otherwise return a non-zero value.
int sd_card_load(dataSet out[], char *filename);

#endif /* SD_CARD_H_ */
