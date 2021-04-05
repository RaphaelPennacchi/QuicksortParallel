#include "sortLibrary.h"
#include <omp.h>

// Speed up the swap call
static inline void swap(int* arr, int i, int j){
  int k = arr[i];
  arr[i] = arr[j];
  arr[j] = k;
}


// Hoare based parallel quicksort
// Create quicksort tasks till the cutoff limit, does not 
// Escalate the tasks till 1 index
void quicksortParallel(int* array, int left, int right, int cutoff) {
  int i = left, j = right;
  // Pivot choosen by randon
  int pivot = array[(rand() % (right - left)) + left];

  // While left index is less than right index run
  while (i <= j) {
    // While left values are less than pivot goes right
    while (array[i] < pivot) i++;
    // While right values are less than pivot goes  left 
    while (array[j] > pivot) j--;
    // If the index o i is less than j change everything 
    if (i <= j) {
      // Make the change and update values
      swap(array, i++, j--);
    }
  }
  // Check if quicksort is in cutoff range
  if (((right - left) < cutoff)) {
    // If the left is less than the holder for right call new range
    if (left < j)   quicksortParallel(array, left, j, cutoff);
    // If the holder for left is less than right call new range
    if (i < right)  quicksortParallel(array, i, right, cutoff);
  }
  else {
    // If it isn't create tasks with it
#pragma omp task 	
    { quicksortParallel(array, left, j, cutoff); }
#pragma omp task 	
    { quicksortParallel(array, i, right, cutoff); }
  }
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  double initTime, endTime;
  Array* myArray;

  myArray = readFile(argv[1]);
  initTime = wtime();
  quicksortParallel(myArray->data, 0, myArray->size - 1, 1000);
  endTime = wtime();
  printTime("Quicksort Parallel", initTime, endTime);
  checkSort(myArray);
  freeArray(myArray);

  return 0;
}