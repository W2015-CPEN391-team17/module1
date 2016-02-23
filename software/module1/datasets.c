/*
 * datasets.c
 *
 * Functions for GPS data (mapped to screen pixels) representing a path
 *
 */

#include "datasets.h"
#include "gps_points.h"

/*
 * Copy the data from the slot with the given index in datasets
 * to workingDataSet
 */
void load_into_workingDataSet(int index) {
	//for the slot with the given index
	dataSet *copyFromSet = &localData.dataSets[index];

	//copy the data
	int size = copyFromSet->size;
	localData.workingDataSet.size = size;
	int i;
	for(i = 0; i < size; i++) {
		localData.workingDataSet.points[i] = copyFromSet->points[i];
	}
}

/*
 * Copy the data from the workingDataSet to the slot with the
 * given index in datasets
 */
void save_from_workingDataSet(int index) {
	//to the slot with the given index
	dataSet *copyToSet = &localData.dataSets[index];

	//copy the data
	int size = localData.workingDataSet.size;
	copyToSet->size = size;
	int i;
	for(i = 0; i < size; i++) {
		copyToSet->points[i] = localData.workingDataSet.points[i];
	}
}

void loadgps_workingDataSet(){
	GPSPoint* screen_points;

	screen_points = convertGPSPoints(70);

	GPSPoint *workingDataPoints = localData.workingDataSet.points;

	int j;
	for( j = 0; j < 70; j++ ){
		workingDataPoints[j] = screen_points[j];
		printf("x %d: %f  y %d: %f\n", j, workingDataPoints[j].x,
									   j, workingDataPoints[j].y);
	}

	localData.workingDataSet.size = 70;
}
