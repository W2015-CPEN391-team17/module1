#ifndef SD_CARD_H_
#define SD_CARD_H_

// TODO:
// Attempt to write an array of GPSPoints to the SD card
// Return a non-zero value on success, otherwise return 0.
int sd_card_write_points(void);

// Attempt to open a FAT16 formatted SD card and write a file named
// "test.txt" to it.
void sd_card_test(void);

#endif /* SD_CARD_H_ */
