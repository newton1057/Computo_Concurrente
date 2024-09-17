#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define MAX_BITS 64
#define N 1999999

long long suma_binaria(long long a, long long b);
long long bin2dec(int* bin);
void dec2bin(long long a, int* bin);
void printBin(int* bin);
void initBin(int* bin);

int main (int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int world_size;
    int world_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) + world_rank);  // Diferenciar la semilla entre procesos

    long int arreglo[N];
    long int suma = 0, parcial;
    int a = 0, j = 0;
    int id = world_rank, i = 0;
    long int valor = 0;
    int fin = (int)log2(world_size);  // Aseguramos que sea entero

    // Inicialización del arreglo en todos los procesos (aunque es innecesario si solo se necesita en world_rank == 0)
    for (a = 0; a < N; a++) {
        arreglo[a] = 10000 + rand() % 1000;
    }

    // Cálculo de suma parcial para cada proceso
    for (j = world_rank; j < N; j += world_size) {
        suma += arreglo[j];
    }

    if (world_size == 1) {
        printf("El resultado de la suma es: %ld\n", suma);
    } else {
        // Proceso de suma binaria en árbol
        while (id % 2 == 0 && i < fin) {
            int rankk = world_rank + (int)pow(2, i);
            MPI_Recv(&valor, 1, MPI_LONG, rankk, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            suma += suma_binaria(suma, valor);
            id /= 2;
            i++;
        }

        if (world_rank != 0) {
            int rankk = world_rank - (int)pow(2, i);
            MPI_Send(&suma, 1, MPI_LONG, rankk, 0, MPI_COMM_WORLD);
        }

        // Solo el proceso 0 imprime el resultado final
        if (world_rank == 0) {
            printf("El resultado de la suma es: %ld\n", suma);
        }
    }

    MPI_Finalize();
    return 0;
}

long long suma_binaria(long long a, long long b) {
    int binA[MAX_BITS], binB[MAX_BITS], binC[MAX_BITS];
    initBin(binA);
    initBin(binB);
    initBin(binC);

    dec2bin(a, binA);
    dec2bin(b, binB);

    int carry = 0, i = 0;
    do {
        int sumaBits = binA[i] + binB[i] + carry;
        binC[i] = sumaBits % 2;
        carry = sumaBits / 2;
        i++;
    } while (i < MAX_BITS);

    return bin2dec(binC);
}

void dec2bin(long long a, int* bin) {
    int i = 0;
    while (a != 0 && i < MAX_BITS) {
        bin[i++] = a % 2;
        a /= 2;
    }
}

long long bin2dec(int* bin) {
    long long decimal = 0, potencia = 1;
    for (int i = 0; i < MAX_BITS; i++) {
        decimal += bin[i] * potencia;
        potencia *= 2;
    }
    return decimal;
}

void initBin(int* bin) {
    for (int i = 0; i < MAX_BITS; i++)
        bin[i] = 0;
}
