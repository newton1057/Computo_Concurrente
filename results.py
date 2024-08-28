import json

def loadJSON():
    with open('results.json', 'r') as file:
        data = json.load(file)

    return data

data = loadJSON()
listAverage = []

for i in data:
    dataExecution = i['dataExecution']
    listTimeExecution = []
    numberThreads = None
    for j in dataExecution:
        print(j)
        numberThreads = j['numberThreads']
        listTimeExecution.append(j['timeExecution'])

    print(f'numberThreads: {numberThreads}')
    average = sum(listTimeExecution) / len(listTimeExecution)
    print (f'Average : {average}')
    listAverage.append(average)
    acceleration = listAverage[0] / average
    print (f'Acceleration : {acceleration}')
    print("")