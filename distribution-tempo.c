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

int array_max(int *v, unsigned int n) {
    int max = v[0];
    for (int i=1;i<n;i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}

int array_min(int *v, unsigned int n) {
    int min = v[0];
    for (int i=1;i<n;i++) {
        if (v[i] < min) {
            min = v[i];
        }
    }
    return min;
}

void distribution_sort(int *v, unsigned int n) {
    int s, b, d, *c, *w;
    s = array_min(v, n);
    b = array_max(v, n);
    w = (int *)malloc(n * sizeof(int));
    c = (int *)malloc((b - s + 1) * sizeof(int));
    for (int i=0;i<b-s+1;i++) {
        c[i] = 0;
    }
    for (unsigned int i = 0; i < n; i++) {
        c[v[i] - s] = c[v[i] - s] + 1;
    }
    for (int i = 1; i < b - s + 1; i++) {
        c[i] = c[i] + c[i - 1];
    }
    for (int i = 0; i < n; i++) {
        d = v[i] - s;
        w[c[d] - 1] = v[i];
        c[d] = c[d] - 1;
    }
    for (unsigned int i = 0; i < n; i++) {
        v[i] = w[i];
    }
    free(c);
    free(w);
}

int main(int argc, char **argv) {
    struct timespec start, end;
    unsigned int t;
    unsigned int n = atoi(argv[1]);
    int i, *v;
    v = (int *)malloc(n * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        v[i] = rand() % 10000;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }

    distribution_sort(v, n);

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }

    t = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
    printf("%d\n", t);
    free(v);
    return EXIT_SUCCESS;
}