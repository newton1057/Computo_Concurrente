import subprocess
import time
import json

def ExecuteC(iteration, numberThreads, numberSet):
    print('Information of the execution: ')
    print(f' - Iteration: {iteration}')
    print(f' - Threads: {numberThreads}')
    print(f' - Set: {numberSet}')
    
    # Capturar el tiempo antes de ejecutar el programa
    timeStart = time.time()

    # Parámetros que deseas pasar al programa en C
    params = [str(numberThreads), str(numberSet)]

    # Ejecutar el programa en C con los parámetros
    result = subprocess.run(['./Practica2'] + params, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)

    # Capturar el tiempo después de ejecutar el programa
    timeEnd = time.time()

    # Calcular el tiempo total de ejecución
    timeExecution = timeEnd - timeStart

    # Mostrar la salida del programa
    # print("Salida del programa en C:")
    # print(result.stdout)

    # Mostrar errores si los hay
    if result.stderr:
        print("Errores:")
        print(result.stderr)

    # Mostrar el tiempo de ejecución
    print(f"Tiempo de ejecución: {timeExecution:.2f} segundos\n")

    return timeExecution

def saveJSON (data):
    with open ('results.json', 'w') as file:
        json.dump(data, file, indent=4)

if __name__ == "__main__":
    listNumberThreads = [1,2,4,8,16,32]
    numberSet = 20
    data = []
    for thread in listNumberThreads:
        
        executions = []
        for iteration in range(5):
            timeExection = ExecuteC(iteration, thread,numberSet)
            execution = {
                'numberThreads' : thread,
                'numberSet' : numberSet,
                'timeExecution' : timeExection
            }
            executions.append(execution)

        dataExecution = {
            'dataExecution' : executions
        }

        data.append(dataExecution)

    saveJSON(data)