#ifndef GPS_POINTS_H_
#define GPS_POINTS_H_

struct points {
	char latitude[16];
	char longitude[16];
	int lat_swapped;
	int long_swapped;
};

int swapEndian(char *s);
char *FloatToLatitudeConversion(int x);
char *FloatToLongitudeConversion(int x);
void read_string(char *output);
void config_log(void);
void save_points(void);
void start_log(void);
void stop_log(void);
void erase_log(void);
void query_log(void);

#endif
