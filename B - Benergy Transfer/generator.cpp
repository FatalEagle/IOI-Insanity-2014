#include <cstdio>
#include <cstdlib>
#include <ctime>

#if RAND_MAX == 32767
  inline size_t rand32(int lo, int hi) {
    return ((bool(rand()&1)<<30) | (rand()<<15)+rand())
            % (hi - lo + 1) + lo;
  }
#else
  inline size_t rand32(int lo, int hi) {
    return rand() % (hi - lo + 1) + lo;
  }
#endif

void gen5(int N = 10) {
  freopen("testdata/5.in", "w", stdout);
  srand(time(0));
  printf("%d\n", N);
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, 1000000)); printf("\n");
}

void gen6(int N = 1000) {
  freopen("testdata/6.in", "w", stdout);
  srand(time(0));
  printf("%d\n", N);
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(800000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(800000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(0, 1)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(0, 1)); printf("\n");
}

void gen7(int N = 10000) {
  freopen("testdata/7.in", "w", stdout);
  srand(time(0));
  printf("%d\n", N);
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", -950000); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", 999999); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", 2); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", 1); printf("\n");
}

void gen8(int N = 50000) {
  freopen("testdata/8.in", "w", stdout);
  srand(time(0));
  printf("%d\n", N);
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-10, 10)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-10, 10)); printf("\n");
}

void gen9(int N = 100000) {
  freopen("testdata/9.in", "w", stdout);
  srand(time(0));
  printf("%d\n", N);
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(900000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(-1000000, -900000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(0, 2)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(0, 2)); printf("\n");
}

void gen10(int N = 100000) {
  freopen("testdata/10.in", "w", stdout);
  srand(time(0));
  printf("%d\n", N);
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", -rand32(900000, 1000000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", rand32(1000000, 900000)); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", 10); printf("\n");
  for (int i = 0; i < N; i++) printf(i ? " %d" : "%d", 1); printf("\n");
}

int main() {
  //gen5();
  return 0;
}
