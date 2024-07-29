/*
**    Programa para ilustrar el uso de los hilos
**    y memoria compartida.
**
**    Compilar con : gcc arreglo_hilos.c -lpthread -o arreglo_hilos
**
*/

// Include libraries to use threads and standard input/output
#include <pthread.h> 
#include <stdio.h>

// Define the size of the array
#define TAM_ARR 10 /* Tamaño PAR del arreglo compartido. */
#define NUM_HILOS 2 /* Número de hilos a crear. */

/* Este arreglo global es COMPARTIDO por todos los hilos. */
int arreglo[TAM_ARR];

/* Esta función es para que los hilos la ejecuten. */
void calculaCuadrados(void *ptrEntero)
{
   /* Calcular cuadrados de la parte que me toca. */
   for (int i = 0; i < TAM_ARR; i++) {
      arreglo[i] = arreglo[i] * arreglo[i];
   }
}

/* Estos dos procedimientos solamente los usa el hilo principal (main). */
void iniciarArreglo(int *arr, int inicio, int fin)
{
   for (int i = inicio; i <= fin; ++i)
      arr[i] = i;
}

void mostrarArreglo(int *arr, int inicio, int fin)
{
   printf("\nEl arreglo: ");
   for (int i = inicio; i <= fin; ++i)
      printf("%d ", arr[i]);

   printf("\n\n");
}


int main()
{
   /**** Comienza solamente un hilo. ****/

   /* El hilo principal Inicializa el arreglo. */
   iniciarArreglo(arreglo, 0, TAM_ARR - 1);

   /* Crear el PRIMER hilo para procesar la primera mitad del arreglo.  */
   
   pthread_t hilo1[NUM_HILOS];

   for (int i = 0; i < NUM_HILOS; i++)
   {
      pthread_create(&hilo1[i], NULL, (void*) calculaCuadrados, NULL);
   }

   /* crear_hilo(...); */

   //calculaCuadrados(NULL);
   /* Crear el SEGUNDO hilo para procesar la segunda mitad del arreglo. */
   /* crear_hilo(...); */

   /* ESTO YA NO LO EJECUTAN LOS HILOS. */

   /* El hilo principal muestra el arreglo final */
   mostrarArreglo(arreglo, 0, TAM_ARR - 1);

   return 0;
}