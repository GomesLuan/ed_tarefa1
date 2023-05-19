#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <malloc.h>

size_t get_allocated_memory(void *ptr) {
    return _msize(ptr);
}
#else
#include <unistd.h>

size_t get_allocated_memory(void *ptr) {
    return malloc_usable_size(ptr);
}
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
    free(w);

    unsigned long c_size = (unsigned long)get_allocated_memory(c);
    printf("%lu\n", c_size);
    free(c);
}

int main(int argc, char **argv) {
    unsigned int n = atoi(argv[1]);
    int i, *v;
    v = (int *)malloc(n * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        v[i] = rand();
    }
    distribution_sort(v, n);
    free(v);
    return 0;
}