/*
 * datasets.h
 *
 *  Created on: 2016-02-19
 *      Author: Kyle
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

#endif /* DATASETS_H_ */
