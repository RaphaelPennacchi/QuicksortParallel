#include "sortLibrary.h"
#include <mpi.h>

static inline void swap(int* arr, int i, int j) {
  int k = arr[i];
  arr[i] = arr[j];
  arr[j] = k;
}

// Hoare based parallel quicksort
// Create quicksort tasks till the cutoff limit, does not 
// Escalate the tasks till 1 index
void quicksortParallel(int* array, int left, int right) {
  int i = left, j = right;
  // Pivot choosen by randon
  if (i >= j) return;
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
  // If the left is less than the holder for right call new range
  if (left < j)   quicksortParallel(array, left, j);
  // If the holder for left is less than right call new range
  if (i < right)  quicksortParallel(array, i, right);
}

// Merge two arrays
int* merge(int* vector1, int size1, int* vector2, int size2)
{
  int* finalVector = (int*)malloc((size1 + size2) * sizeof(int));
  int i, j, k;
  i = j = k = 0;
  while (i < size1 && j < size2) {
    if (vector1[i] < vector2[j]) finalVector[k++] = vector1[i++];
    else finalVector[k++] = vector2[j++];
  }
  while(i < size1){
    finalVector[k++] = vector1[i++];
  }
  while(j < size2){
    finalVector[k++] = vector2[i++];
  }
  return finalVector;
}


int main(int argc, char** argv) {
  srand(time(NULL));
  double initTime, endTime;
  Array* myArray;
  int rank, size, chunkSize, * chunk;
  int range, receiveChunkSize, * receiveChunkBuffer;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  // Verify number of args
  if (argc < 2) {
    puts("Entry format: mpirun -n <numOfProcessors> <fileName> <entryFileName>");
    goto Final;
  }

  // If process 0 read the file
  if (!rank) {
    myArray = readFile(argv[1]);
  }
  else {
    myArray = (Array*)malloc(sizeof(Array));
  }
  initTime = MPI_Wtime();
  // Broadcast the myArray->size to every process
  MPI_Bcast(&myArray->size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // printf("Rank %d Vector size %d\n", rank, myArray->size);
  // If myArray->size % size != 0 need to add 1 to the division else use the division
  // Size of the chunks that are going to be sent to each process
  chunkSize = (myArray->size % size) ? myArray->size / size + 1 : myArray->size / size;

  //Allocate Chunk
  chunk = (int*)malloc(sizeof(int) * chunkSize);
  //Scatter
  MPI_Scatter(myArray->data, chunkSize, MPI_INT, chunk, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
  if (!rank) {
    free(myArray->data);
  }

  range = (myArray->size >= chunkSize * (rank + 1)) ? chunkSize : myArray->size - chunkSize * rank;
  quicksortParallel(chunk, 0, range - 1);
  // for (int i = 0; i < range; i++) {
  //   printf("Rank %d number %d\n", rank, chunk[i]);
  // }

  MPI_Barrier(MPI_COMM_WORLD);

  // up to log_2 p merge steps
  for (int step = 1; step < size; step = 2 * step) {
    if (rank % (2 * step) != 0) {
      // rank is no multiple of 2*step: send chunk to rank-step and exit loop
      MPI_Send(chunk, range, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
      break;
    }
    // rank is multiple of 2*step: merge in chunk from rank+step (if it exists)
    if (rank + step < size) {
      // Compute size of chunk to be received
      receiveChunkSize = (myArray->size >= chunkSize * (rank + 2 * step)) ? chunkSize * step : myArray->size - chunkSize * (rank + step);
      // Receive other chunk
      receiveChunkBuffer = (int*)malloc(sizeof(int) * receiveChunkSize);
      MPI_Recv(receiveChunkBuffer, receiveChunkSize, MPI_INT, rank + step, 0, MPI_COMM_WORLD, NULL);
      myArray->data = merge(chunk, range, receiveChunkBuffer, receiveChunkSize);
      free(chunk);
      free(receiveChunkBuffer);
      chunk = myArray->data;
      range = range + receiveChunkSize;
    }
  }
  endTime = MPI_Wtime();

  if (!rank) {
    printTime("Quicksort MPI", initTime, endTime);
    myArray->data = chunk;
    checkSort(myArray);
  }

Final:
  MPI_Finalize();
  return 0;
}
