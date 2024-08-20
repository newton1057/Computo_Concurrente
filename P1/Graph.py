import matplotlib.pyplot as plt

# Datos
hilos = [1, 2, 3, 4]
tiempos = [10.00, 5.50, 4.00, 3.20]  # Tiempo de ejecución en segundos
aceleracion = [1.00, 1.82, 2.50, 3.13]

# Crear gráfica
plt.figure(figsize=(10, 5))

# Gráfica de tiempo de ejecución
plt.subplot(1, 2, 1)
plt.plot(hilos, tiempos, marker='o')
plt.xlabel('Número de Hilos')
plt.ylabel('Tiempo de Ejecución (s)')
plt.title('Tiempo de Ejecución vs. Número de Hilos')

# Gráfica de aceleración
plt.subplot(1, 2, 2)
plt.plot(hilos, aceleracion, marker='o', color='r')
plt.xlabel('Número de Hilos')
plt.ylabel('Aceleración')
plt.title('Aceleración vs. Número de Hilos')

plt.tight_layout()
plt.show()