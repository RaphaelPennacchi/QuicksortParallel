#include "sortLibrary.h"

static inline void swap(int* arr, int i, int j){
  int k = arr[i];
  arr[i] = arr[j];
  arr[j] = k;
}

// Lamuto sort
// Cormem implementation of quicksort
int partition(int* arr, int left, int right) {
  int point = arr[right];
  int i = left - 1;
  for (int j = left; j < right; j++) {
    if (arr[j] <= point) {
      i++;
      swap(arr, i, j);
    }
  }
  swap(arr, i + 1, right);
  return i + 1;
}

int randomPartition(int* arr, int left, int right) {
  int random = (rand() % (right - left)) + left;
  swap(arr, random, left);
  return partition(arr, left, right);
}


void quicksort(int* arr, int left, int right) {
  if (left >= right) return;
  int q = partition(arr, left, right);
  quicksort(arr, left, q - 1);
  quicksort(arr, q + 1, right);
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  double initTime, endTime;
  Array* myArray;



  myArray = readFile(argv[1]);
  initTime = wtime();
  quicksort(myArray->data, 0, myArray->size - 1);
  endTime = wtime();
  printTime("Quicksort Sequencial ", initTime, endTime);
  checkSort(myArray);
  freeArray(myArray);

  return 0;
}
