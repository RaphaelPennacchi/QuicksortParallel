#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const* argv[]) {
  srand(time(NULL));
  char buffer[50];
  unsigned long long int size = atoi(argv[1]);
  sprintf(buffer, "vector-%llu.map", size);
  FILE* myFile = fopen(buffer, "w");
  fprintf(myFile, "#Linha#\n%llu#\n", size);
  while (size--) {
    fprintf(myFile, "%d\n", rand() % 10);
  }
  fclose(myFile);
  return 0;
}
