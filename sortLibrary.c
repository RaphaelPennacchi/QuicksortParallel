#include "sortLibrary.h"

double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + (double)t.tv_usec / 1000000;
}

void printTime(char* string, double init, double end) {
  printf("%s: %lf\n", string, end - init);
}

void printArray(Array* myArray) {
  for (int i = 0; i < myArray->size; i++) printf("%d", myArray->data[i]);
  puts("");
}

void freeArray(Array* myArray) {
  free(myArray->data);
  free(myArray);
}

void checkSort(Array* myArray) {
  for (int i = 1; i < myArray->size; i++) {
    if (myArray->data[i] < myArray->data[i - 1]) {
      printf("Not sorted, error at indice %d\n", i);
      return;
    }
  }
  puts("Sorted");
}

Array* readFile(char* fileName) {
  int arraySize;
  FILE* myFile = fopen(fileName, "r");

  fscanf(myFile, "#Linha#\n%d#\n", &arraySize);

  Array* myArray = (Array*)malloc(sizeof(Array));
  myArray->size = arraySize;
  myArray->data = (int*)malloc(sizeof(int) * arraySize);
  for (unsigned long long int i = 0; i < arraySize; i++) fscanf(myFile, "%d", &(myArray->data[i]));

  return myArray;
}

// void swap(int* arr, int i, int j) {
//   int k = arr[i];
//   arr[i] = arr[j];
//   arr[j] = k;
// }