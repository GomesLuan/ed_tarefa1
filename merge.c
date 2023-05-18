#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

void merge(int *v, unsigned int s, unsigned int m, unsigned int e) {
    int i, *w;
    w = (int *)malloc((e-s+1) * sizeof(int));
    unsigned int p = s;
    unsigned int q = m + 1;
    for (i=0;i<(e-s+1);i++) {
        if ((q > e) || ((p <= m) && (v[p] < v[q]))) {
            w[i] = v[p];
            p++;
        }
        else {
            w[i] = v[q];
            q++;
        }
    }
    for (i=0;i<(e-s+1);i++) {
        v[s+i] = w[i];
    }
    free(w);
}

void merge_sort(int *v, unsigned int s, unsigned int e) {
  if (s < e) {
    unsigned int m = floor((s + e)/2);
    merge_sort(v, s, m);
    merge_sort(v, m+1, e);
    merge(v, s, m, e);
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

    merge_sort(v, 0, n-1);

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }

    t = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    printf("%d\n", t);
    free(v);
    return EXIT_SUCCESS;
}