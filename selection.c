#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    // Definições para Windows
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    int clock_gettime(int dummy, struct timespec *spec) {
        (void)dummy;  // Evita avisos de variável não utilizada

        static LARGE_INTEGER frequency;
        LARGE_INTEGER currentTime;

        if (!QueryPerformanceFrequency(&frequency))
            return -1;

        if (!QueryPerformanceCounter(&currentTime))
            return -1;

        spec->tv_sec = currentTime.QuadPart / frequency.QuadPart;
        spec->tv_nsec = (long)(((currentTime.QuadPart % frequency.QuadPart) * 1e9) / frequency.QuadPart);

        return 0;
    }

    #define CLOCK_MONOTONIC 0
#else
    // Definições para sistemas Unix
    #include <unistd.h>
    #include <errno.h>
#endif

void swap(int *a, int *b) {
  int m;
  m = *a;
  *a = *b;
  *b = m;
}

void selection_sort(int *v, unsigned int n) {
  unsigned int i, j;
  int m;
  for (i = 0; i < (n - 1); i++) {
    m = i;
    for (j = (i + 1); j < n; j++) {
      if (v[m] > v[j]) {
        m = j;
      }
    }
    swap(&v[m], &v[i]);
  }
}

int main(int argc, char **argv) {
    struct timespec start, end;
    unsigned int t;
    unsigned int n = atoi(argv[1]);
    int i, *v;
    v = (int *)malloc(n * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < n; i++)
        v[i] = rand();

    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }

    selection_sort(v, n);

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }

    t = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    printf("%d\n", t);
    free(v);
    return EXIT_SUCCESS;
}