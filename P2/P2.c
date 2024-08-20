#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define MAX_SET_SIZE 100

int set[MAX_SET_SIZE];
int set_size;
int num_threads;
int h;
long long total_subsets = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadArgs {
    int thread_id;
    int base_sum;
};

void* thread_work(void* arg) {
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int thread_id = args->thread_id;
    int base_sum = args->base_sum;
    int subset_size = set_size - h;
    long long local_count = 0;

    for (long long i = 0; i < (1LL << subset_size); i++) {
        int sum = base_sum;
        for (int j = 0; j < subset_size; j++) {
            if (i & (1LL << j)) {
                sum += set[j];
            }
        }
        if (sum == 0) {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    total_subsets += local_count;
    pthread_mutex_unlock(&mutex);

    free(arg);
    return NULL;
}

int calculate_base_sum(long long bits_base) {
    int sum = 0;
    for (int i = 0; i < h; i++) {
        if (bits_base & (1LL << i)) {
            sum += set[set_size - h + i];
        }
    }
    return sum;
}

int main() {
    printf("Ingrese el número de hilos (debe ser potencia de 2): ");
    scanf("%d", &num_threads);

    printf("Ingrese el tamaño del conjunto: ");
    scanf("%d", &set_size);

    printf("Ingrese los elementos del conjunto:\n");
    for (int i = 0; i < set_size; i++) {
        scanf("%d", &set[i]);
    }

    h = log2(num_threads);

    pthread_t threads[num_threads];
    long long bits_base = 0;

    clock_t start = clock();

    for (int t = 0; t < num_threads; t++) {
        struct ThreadArgs* args = malloc(sizeof(struct ThreadArgs));
        args->thread_id = t;
        args->base_sum = calculate_base_sum(bits_base);

        pthread_create(&threads[t], NULL, thread_work, args);

        bits_base++;
    }

    for (int t = 0; t < num_threads; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Número total de subconjuntos que suman 0: %lld\n", total_subsets);
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    return 0;
}