#include <mpi.h>
#include <stdio.h>

#define N 11  // Total de elementos a repartir

int main(int argc, char* argv[])
{
    // Inicializa el entorno MPI
    MPI_Init(&argc, &argv);

    int miRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

    int numProcs; 
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    // Cálculo de la repartición de elementos
    int base_elements = N / numProcs;  // Elementos base por proceso
    int extra_elements = N % numProcs;  // Elementos extra a repartir

    int my_elements;  // Número de elementos que le toca a este proceso

    if (miRank < extra_elements) {
        my_elements = base_elements + 1;
    } else {
        my_elements = base_elements;
    }

    // Mostrar cuántos elementos le tocan a cada proceso
    printf("Soy el proceso %d, me tocan %d elementos \n", miRank, my_elements);

    MPI_Finalize();

    return 0;
}