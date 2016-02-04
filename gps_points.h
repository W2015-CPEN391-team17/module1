#ifndef GPS_POINTS_H_
#define GPS_POINTS_H_

struct points {
	char latitude[10];
	char longitude[10];
	int lat_swapped;
	int long_swapped;
};

int swapEndian(char *s);
void FloatToLatitudeConversion(int x, char *latitude);
void FloatToLongitudeConversion(int x, char *longitude);
void read_string(char *output);
void config_log(void);
void dump_log(void);
void start_log(void);
void stop_log(void);
void erase_log(void);
void query_log(void);

#endif
