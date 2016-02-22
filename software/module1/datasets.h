/*
 * datasets.h
 *
 * Structures for GPS data (mapped to screen pixels) representing a path
 *
 */

#include "conversion.h"

#ifndef DATASETS_H_
#define DATASETS_H_

#define MAX_N_POINTS 1024

//assume double-digit at most (aka <= 99)
#define MAX_N_SETS 10

typedef struct {
	int size; //number of points in the set
	GPSPoint points[MAX_N_POINTS]; //
} dataSet;

typedef struct {
	dataSet workingDataSet; //data currently being displayed
	dataSet dataSets[MAX_N_SETS]; //cache for SD contents
} localDataSets;


/*
 * Copy the data from the slot with the given index in datasets
 * to workingDataSet
 */
void load_into_workingDataSet(int index);

/*
 * Copy the data from the workingDataSet to the slot with the
 * given index in datasets
 */
void save_from_workingDataSet(int index);

#endif /* DATASETS_H_ */
