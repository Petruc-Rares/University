// Copyright 2019 Petruc Rares

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "include/project.h"

// function which frees memory reserved by a matrix
void free_matrix(int **matrix, int matrixSize) {
  int i;
  for (i = 0; i < matrixSize; i++) {
    free(matrix[i]);
  }
  free(matrix);
  return;
}

int *arrayProduct(int *array, int arrayLength) {
  int i, j;
  int* product_array = (int*)calloc(arrayLength, sizeof(int));
  for (i = 0; i < arrayLength; i++) {
    product_array[i] = 1;
    for (j = 0; j < arrayLength; j++) {
      if (i != j) {
        product_array[i] *= array[j];
      }
    }
  }
    return product_array;
}

int **rotateMatrix(int **matrix, int matrixSize) {
  int i, j;
  int **rotated_matrix = (int**)calloc(matrixSize, sizeof(int*));
  for (i = 0; i < matrixSize; i++) {
    rotated_matrix[i] = (int*)calloc(matrixSize, sizeof(int));
  }
  for (j = 0; j < matrixSize; j++) {
    for (i = 0; i < matrixSize; i++) {
      rotated_matrix[i][j] = matrix[j][matrixSize-i-1];
    }
  }
    return rotated_matrix;
}

int *subMatrixesSums(int **matrix, int queriesNo, int *queries) {
  int i, queryNb, j, x1Pos = 0, x2Pos = 2, y1Pos = 1,
      y2Pos = 3, querySize = 4; // Pos = Position
  int *queries_sum = (int*)calloc(queriesNo, sizeof(int));
  for (queryNb = 0; queryNb < queriesNo; queryNb++) {
    queries_sum[queryNb] = 0;
    for (i = queries[queryNb * querySize + x1Pos];
         i <= queries[queryNb * querySize + x2Pos]; i++) {
      for (j = queries[queryNb * querySize + y1Pos];
           j <= queries[queryNb * querySize + y2Pos]; j++) {
         queries_sum[queryNb] += matrix[i][j];
      }
    }
  }
  return queries_sum;
}

TDriver *allocDriver(int ridesNo) {
  TDriver *driver = (TDriver *)calloc(1, sizeof(TDriver));
  driver -> ridesNb = ridesNo;
  driver -> v = (TRide *) calloc(ridesNo, sizeof(TRide));
  return driver;
}

TDriver **allocDrivers(int driversNo, int *driversRidesNo) {
  int i;
  TDriver **drivers = (TDriver **)calloc(driversNo, sizeof(TDriver*));
  for (i = 0; i < driversNo; i++) {
    drivers[i] = (TDriver*)allocDriver(driversRidesNo[i]);
  }
  return drivers;
}

TDriver **readDrivers(FILE *inputFile, int *driversNo) {
  int driversNb, i, j;
  fread(&driversNb, sizeof(int), 1, inputFile); // drivers number
  int *driversRide = calloc(driversNb, sizeof(int));
  *driversNo = driversNb;
  // array representing number of races for each driver
  fread(driversRide, sizeof(int), driversNb, inputFile);
  // allocation of drivers array of structs
  TDriver **drivers = allocDrivers(driversNb, driversRide);
  for (i = 0; i < driversNb; i++) {
    fread(&(drivers[i]->name), sizeof(drivers[i]->name), 1, inputFile);
    fread(&(drivers[i]->plateNb), sizeof(drivers[i]->plateNb), 1, inputFile);
    fread(&(drivers[i]->x), sizeof(drivers[i]->x), 1, inputFile);
    fread(&(drivers[i]->y), sizeof(drivers[i]->y), 1, inputFile);
    for (j = 0; j < driversRide[i]; j++) {
      fread(&(drivers[i]->v[j].x), sizeof(drivers[i]->v[j].x), 1, inputFile);
      fread(&(drivers[i]->v[j].y), sizeof(drivers[i]->v[j].y), 1, inputFile);
      fread(&(drivers[i]->v[j].rideState),
            sizeof(drivers[i]->v[j].rideState), 1, inputFile);
    }
  }
  free(driversRide);
  return drivers;
}

void printDrivers(FILE *outputFile, TDriver **drivers, int driversNo) {
  unsigned int j;
  int i;
  for (i = 0; i < driversNo; i++) {
    fwrite(&(drivers[i]->name), sizeof(drivers[i]->name), 1, outputFile);
    fwrite(&(drivers[i]->plateNb), sizeof(drivers[i]->plateNb), 1, outputFile);
    fwrite(&(drivers[i]->x), sizeof(drivers[i]->x), 1, outputFile);
    fwrite(&(drivers[i]->y), sizeof(drivers[i]->y), 1, outputFile);
    for (j = 0; j < (drivers[i] -> ridesNb); j++) {
      fwrite(&(drivers[i]->v[j].x), sizeof(drivers[i]->v[j].x), 1, outputFile);
      fwrite(&(drivers[i]->v[j].y), sizeof(drivers[i]->v[j].y), 1, outputFile);
      fwrite(&(drivers[i]->v[j].rideState),
             sizeof(drivers[i]->v[j].rideState), 1, outputFile);
    }
  }
}

char *maxRatingDriverName(TDriver **drivers, int driversNo) {
  int starSum, denominator, i, auxi;
  unsigned int j;
  double rating, max = -1;
  for (i = 0; i < driversNo; i++) {
    // reboot starSum and denominator at every driver
    starSum = 0;
    denominator = 0;
    for (j = 0; j < (drivers[i] -> ridesNb); j++) {
      starSum += GET_STARS(drivers[i]->v[j].rideState);
      if (CANCELED_RIDE(drivers[i]->v[j].rideState) != 0) denominator--;
      else if (ONGOING_RIDE(drivers[i]->v[j].rideState) != 0) denominator--;
    }
    denominator += (drivers[i] -> ridesNb);
    rating = (double)starSum/denominator;
    if (rating > max) {
      max = rating;
      auxi = i; // auxi receives the index for the wanted driver
    }
  }
  return (drivers[auxi]->name);
}

// calculation of Euclidean distance
double get_distance(double desiredX, double driverX,
                     double desiredY, double driverY) {
  return sqrt(pow((desiredX - driverX), 2) + pow((desiredY - driverY), 2));
}

// function used for swapping ints
void swapInt(int *x, int *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

// function used for swapping doubles
void swapDouble(double *x, double *y) {
  double temp = *x;
  *x = *y;
  *y = temp;
}

// function used for sorting distances in ascending order + awareness of index
void sortAscending(double *driverDistance, int *indexDriver, int driversNo) {
  int i, j;
  for (i = 0; i < driversNo - 1; i++) {
    for (j = i + 1; j < driversNo; j++) {
      if (driverDistance[i] > driverDistance[j]) {
        swapDouble(&driverDistance[i], &driverDistance[j]);
        swapInt(&indexDriver[i], &indexDriver[j]);
      }
    }
  }
  return;
}

// function used for swapping structs
void swapStructs(TDriver *a, TDriver *b) {
  TDriver c;
  c = *a;
  *a = *b;
  *b = c;
}

// at equal distances, the order is establised by the "smallest" name
void sortLexicoGraphical(double *driverDistance,
                         TDriver **ClosestDrivers, int resultsNo) {
  int i, j;
  for (i = 0; i < resultsNo - 1; i++) {
    for (j = i + 1; j < resultsNo; j++) {
      if (driverDistance[i] == driverDistance[j]) {
        if (strcmp(ClosestDrivers[i] -> name, ClosestDrivers[j] -> name) > 0) {
          swapStructs(ClosestDrivers[i], ClosestDrivers[j]);
        }
      }
    }
  }
}

TDriver **getClosestDrivers(TDriver **drivers, int driversNo, double desiredX,
    double desiredY, int resultsNo) {
  int i;
  TDriver **ClosestDrivers = (TDriver **)calloc(resultsNo, sizeof(TDriver*));
  int *indexDriver = (int*)calloc(driversNo, sizeof(int));
  double *driverDistance = (double*)calloc(driversNo, sizeof(double));
  for (i = 0; i < driversNo; i++) {
    driverDistance[i] = get_distance(desiredX, drivers[i]->x,
                                     desiredY, drivers[i]->y);
    indexDriver[i] = i;
  }
  sortAscending(driverDistance, indexDriver, driversNo);
  for (i = 0; i < resultsNo; i++) {
    ClosestDrivers[i] = drivers[indexDriver[i]];
  }
  free(indexDriver);
  sortLexicoGraphical(driverDistance, ClosestDrivers, resultsNo);
  free(driverDistance);
  return ClosestDrivers;
}

void freeDriver(TDriver *driver) {
  free(driver->v);
  free(driver);
}

void freeDrivers(TDriver **drivers, int driversNo) {
  int i;
  for (i = 0; i < driversNo; i++) {
    freeDriver(drivers[i]);
  }
  free(drivers);
}

