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
    MPI_Init( &argc , &argv);
    int world_size;
    int world_rank;
    srand(time(NULL));
    long int arreglo[N];
    long int suma, parcial;
    int a=0,j=0;
    int id=world_rank, i=0;
    long int valor = world_rank;
    int fin=log2(world_size);

    MPI_Comm_size( MPI_COMM_WORLD , &world_size);
    MPI_Comm_rank( MPI_COMM_WORLD , &world_rank);
    
    if(world_size > 1){
        if(world_rank==0 || world_rank != 0){
            for (a=0; a < N; a++) {
                arreglo[a] = 10000 + rand()%1000;
            }
            suma=0;
            while(j<N){
                suma = suma+arreglo[j];
                j=j+world_size;
            
            }   
        }
    }else{
        while(j<N){
            parcial= arreglo[j];
            long int suma1 = suma_binaria(suma, parcial);
            suma = suma+ suma1;
            j=j+1;
        }
    }

    if(world_size==1){
         printf("El resultado de la suma es: %d\n", suma);
    }
  
    while(id % 2 ==0 && i< fin ){
        int rankk= world_rank + pow(2,i);
        MPI_Recv(&valor, 1, MPI_LONG, rankk, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        long int valor2 = suma_binaria(suma, valor);
        suma = suma + valor2;
        id=id/2;
        i=i+1;
    }
    if(world_rank !=0){
        int rankk= world_rank - pow(2,i);
        MPI_Send(&suma, 1, MPI_LONG, rankk, 0, MPI_COMM_WORLD);
        
    }
    if(world_rank==0 && world_size>1){
        printf("El resultado de la suma es: %d\n", suma);
    }
    

MPI_Finalize();
return(0);

}

long long suma_binaria(long long a, long long b) {
    // Convertir a y b a binario
    int binA[MAX_BITS];
    int binB[MAX_BITS];
    int binC[MAX_BITS];

    initBin(binA);
    initBin(binB);
    initBin(binC);

    dec2bin(a, binA);
    dec2bin(b, binB);
    
    //printBin(binA);
    //printBin(binB);

    int res = 0;
    int i = 0;
     do {
        int sumaBits = binA[i] + binB[i] + res;
        binC[i] = sumaBits % 2;
        res = sumaBits / 2;
        i++;
    } while (i < MAX_BITS);

    //printBin(binC);

    long long resultado = bin2dec(binC);

    return resultado;
}

void dec2bin(long long a, int* bin) {
    int i = 0;
    while (a != 0 && i < MAX_BITS) {
        long long residuo = a % 2;
        bin[i] = residuo;
        a = a/2;
        i++;
    }
}

long long bin2dec(int* bin) {
    long long potencia = 1;
    long long decimal = 0;
    size_t i;
    for ( i = 0; i < MAX_BITS; i++)
    {
        decimal = decimal + bin[i]*potencia;
        potencia *= 2;
    }

    return decimal;
}

void printBin(int* bin) {
    printf("\n");
    size_t i;
    for (i = 0; i < MAX_BITS; i++)
        printf("%d", bin[i]);
}

void initBin(int* bin) {
    size_t i;
    for ( i = 0; i < MAX_BITS; i++)
        bin[i] = 0;
}