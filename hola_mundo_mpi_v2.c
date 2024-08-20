#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    /* No debe haber llamadas MPI antes de invocar Init */
    MPI_Init(&argc, &argv);

    int miRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

    int numProcs; 
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    // Código que ejecutarán los procesos
    printf("\nHola mundo. Yo soy el proceso %d de %d procesos y me llamo %s \n",miRank, numProcs,processor_name);

    MPI_Finalize();

    /* No debe haber llamadas MPI después de esto. */
    return 0;
}

