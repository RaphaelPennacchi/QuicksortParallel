#ifndef SORTLIBRARY_H
#define SORTLIBRARY_H

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Array {
  int* data;
  int size;
} Array;

// void swap(int*, int, int);

double wtime();
void printTime(char*, double, double);
void printArray(Array*);
void freeArray(Array*);
void checkSort(Array*);
Array* readFile(char*);

#endif 

