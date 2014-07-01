#include <cstdio>

int main(int argc, char **argv) {
  FILE* fans = fopen(argv[1], "r");
  FILE* fout = fopen(argv[2], "r");

  int A, B, ansA, ansB;

  fscanf(fans, "%d%d", &ansA, &ansB);
  if (fscanf(fout, "%d%d", &A, &B) != 2)
    return 1;

  if (A == ansA && B != ansB) {
    printf("4\n10\n");
    return 1;
  }
  if (A != ansA || B != ansB) {
    return 1;
  }
  
  return 0;
}
