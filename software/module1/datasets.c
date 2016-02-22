/*
 * datasets.c
 *
 * Functions for GPS data (mapped to screen pixels) representing a path
 *
 */

#include "datasets.h"

/*
 * Copy the data from the slot with the given index in datasets
 * to workingDataSet
 */
void load_into_workingDataSet(int index) {
	//for the slot with the given index
	dataSet copyFromSet = localData.dataSets[index];

	//copy the data
	int size = copyFromSet.size;
	localData.workingDataSet.size = size;
	int i;
	for(i = 0; i < size; i++) {
		//TODO
	}
}

/*
 * Copy the data from the workingDataSet to the slot with the
 * given index in datasets
 */
void save_from_workingDataSet(int index) {
	//to the slot with the given index
	//copy the size
	//copy each point
}
