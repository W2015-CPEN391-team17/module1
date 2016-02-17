#ifndef SD_CARD_H_
#define SD_CARD_H_

#include "conversion.h"

// Print file contents of a file corresponding to a given filename.
void sd_card_print_contents(char *filename);

// Attempt to write a GPSPoint value to the SD card.
void sd_card_write_point(GPSPoint point, char *filename);

#endif /* SD_CARD_H_ */
