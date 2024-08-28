#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int *set;  // Conjunto de entrada
int n;     // Tamaño del conjunto
int h;     // Número de bits base
int T;     // Número de hilos
int subset_count = 0; // Contador de subconjuntos que suman 0
pthread_mutex_t mutex;  // Para proteger el acceso a recursos compartidos

// Estructura para almacenar un subconjunto
typedef struct {
    int *elements;
    int size;
} Subset;

// Estructura para pasar parámetros a los hilos
typedef struct {
    int base_sum;  // St: suma inicial correspondiente a los bits base
    int thread_id; // Identificador del hilo, que se corresponde con bitsBase
} ThreadParams;

Subset *zero_subsets = NULL;
int zero_subset_count = 0;

void add_zero_subset(int *subset, int size) {
    pthread_mutex_lock(&mutex);

    zero_subsets = realloc(zero_subsets, (zero_subset_count + 1) * sizeof(Subset));
    zero_subsets[zero_subset_count].elements = malloc(size * sizeof(int));
    zero_subsets[zero_subset_count].size = size;
    
    for (int i = 0; i < size; i++) {
        zero_subsets[zero_subset_count].elements[i] = subset[i];
    }

    zero_subset_count++;
    subset_count++;

    pthread_mutex_unlock(&mutex);
}

void print_zero_subsets() {
    printf("Número de subconjuntos que suman 0: %d\n", subset_count);
    for (int i = 0; i < zero_subset_count; i++) {
        printf("{ ");
        for (int j = 0; j < zero_subsets[i].size; j++) {
            printf("%d ", zero_subsets[i].elements[j]);
        }
        printf("}\n");
    }
}

void *generate_subsets(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    int base_sum = params->base_sum; // St: suma inicial
    int thread_id = params->thread_id;

    int subset[32];  // Suponemos un tamaño máximo de conjunto de 32
    int subset_size;

    int total_subsets = 1ULL << (n - h);  // Número de cadenas binarias con |E| - h bits

    for (int i = 0; i < total_subsets; i++) {
        subset_size = 0;
        int current_sum = base_sum;

        // Generar subconjunto a partir de la cadena binaria de |E| - h bits
        for (int j = 0; j < (n - h); j++) {
            if (i & (1 << j)) {
                current_sum += set[j];
                subset[subset_size++] = set[j];
            }
        }

        // Agregar los elementos correspondientes a los bits base
        for (int j = 0; j < h; j++) {
            if (thread_id & (1 << j)) {
                subset[subset_size++] = set[n - h + j];
            }
        }

        // Verificar si la suma actual es 0
        if (current_sum == 0 && subset_size > 0) {
            add_zero_subset(subset, subset_size);
        }
    }

    pthread_exit(NULL);
}



int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <numero_hilos> <tamaño_conjunto>\n", argv[0]);
        return 1;
    }

    T = atoi(argv[1]);  // Número de hilos
    n = atoi(argv[2]);  // Tamaño del conjunto
    h = log2(T);  // h es log2(T)

    if ((1 << h) != T || n <= h) {
        printf("Error: El número de hilos debe ser una potencia de 2 y n debe ser mayor que h.\n");
        return 1;
    }

    set = malloc(n * sizeof(int));

    if (set == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Inicializar la semilla para la generación de números aleatorios
    srand(time(NULL));

    // Llenar el conjunto con números aleatorios entre -100 y 100
    for (int i = 0; i < n; i++) {
        set[i] = rand() % 21 - 10;  // Genera números aleatorios entre -50 y 50
    }

    /* 
    printf("Ingresa los elementos del conjunto (uno por línea):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &set[i]);
    }
    */

   // Imprimir los elementos del conjunto
    
    printf("Elementos del conjunto generados aleatoriamente:\n");
    for (int i = 0; i < n; i++) {
        printf("%d | ", set[i]);
    }
     
    

    pthread_t threads[T];
    ThreadParams thread_params[T];

    pthread_mutex_init(&mutex, NULL);

    int bitsBase = 0;  // Cadena binaria de h bits en ceros

    // Crear los hilos
    for (int i = 0; i < T; i++) {
        thread_params[i].base_sum = 0;

        // Calcular St sumando los elementos correspondientes a bitsBase
        for (int j = 0; j < h; j++) {
            if (bitsBase & (1 << j)) {
                thread_params[i].base_sum += set[n - h + j];
            }
        }

        thread_params[i].thread_id = bitsBase;

        pthread_create(&threads[i], NULL, generate_subsets, (void *)&thread_params[i]);

        bitsBase++;  // Incrementar bitsBase en 1
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    print_zero_subsets();

    for (int i = 0; i < zero_subset_count; i++) {
        free(zero_subsets[i].elements);
    }
    free(zero_subsets);
    free(set);
    pthread_mutex_destroy(&mutex);

    return 0;
}