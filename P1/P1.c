#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long long int addTotal = 0;

// Definir una estructura para agrupar los parámetros
typedef struct {
    int id;
    int start;
    int end;
    long long int *array;
} ThreadArgs;

void initArray(long long int *arr, int start, int end) {
    for (int i = start; i <= end; i++) {
        arr[i] = i + 1;
    }
}

void printArray(long long int *arr, int start, int end) {
    printf("\nArray: ");
    for (int i = start; i <= end; i++) {
        printf("%lld ", arr[i]);
    }
    printf("\n\n");
}

void* addArray(void* arg) {
    // Convertir el puntero void* a un puntero a ThreadArgs
    ThreadArgs *args = (ThreadArgs *)arg;
    int id = args->id;
    int start = args->start;
    int end = args->end;
    long long int *array = args->array;

    printf("Thread %d: start = %d, end = %d\n", id, start, end);

    for (int i = start; i <= end; i++) {
        addTotal += array[i];
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Use: %s <Size array N> <Number of threads H>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int H = atoi(argv[2]);

    printf("Size array N = %d\n", N);
    printf("Number of threads H = %d\n", H);
    
    long long int *arrayNumbers = malloc(N * sizeof(long long int));

    if (arrayNumbers == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    initArray(arrayNumbers, 0, N - 1);
    //printArray(arrayNumbers, 0, N - 1);

    pthread_t threads[H];
    ThreadArgs threadArgs[H];

    int chunkSize = N / H;

    printf("Chunk size = %d\n", chunkSize);

    for (int i = 0; i < H; i++) {
        threadArgs[i].id = i;
        threadArgs[i].start = i * chunkSize;
        threadArgs[i].end = (i == H - 1) ? (N - 1) : (threadArgs[i].start + chunkSize - 1);
        threadArgs[i].array = arrayNumbers;
        pthread_create(&threads[i], NULL, addArray, (void *)&threadArgs[i]);
    }

    for (int i = 0; i < H; i++) {
        pthread_join(threads[i], NULL);
    }

    // printArray(arrayNumbers, 0, N - 1);

    printf("Total = %lld\n", addTotal);

    long long int S = (long long int) N;
    long long int addTotalExpected = (S * (S + 1)) / 2;

    printf("Total expected = %lld\n", addTotalExpected);

    free(arrayNumbers);

    return 0;
}