MANUAL DE USUARIO

INTRODUCCION

Este software simula un sistema operativo básico con tres componentes esenciales:
Gestión de procesos: Registrar, modificar, buscar y eliminar procesos.
Planificación de CPU: Planificador por prioridad que ejecuta procesos según su importancia.
Gestión de memoria: Simula la asignación y liberación de bloques de memoria en forma de pila.

OBJETIVO DEL SISTEMA

El objetivo es brindar una simulación educativa del funcionamiento interno de componentes de un sistema operativo. Este sistema puede servir como herramienta de aprendizaje o base para proyectos más complejos.

Al ejecutar el programa, verás este menú:

OPCIONES DE MENU
1. Insertar nuevo proceso
Descripción: Crea un nuevo proceso y lo agrega a la lista y cola de prioridad.
Pasos:
Ingresar el nombre del proceso (máximo 31 caracteres).
Ingresar la prioridad (0 = alta, números mayores = menor prioridad).
Ingresar la memoria requerida (en bytes).
Ingresar el tiempo de ejecución requerido.
Notas:
El PID se asigna automáticamente.
Si no hay suficiente memoria disponible, se muestra un mensaje de error.
2. Mostrar todos los procesos
Descripción: Muestra una tabla con todos los procesos registrados, incluyendo PID, nombre, prioridad, memoria requerida y tiempo restante.
3. Buscar proceso (por ID o nombre)
Descripción: Busca un proceso por su PID o nombre.
Pasos:
Ingresar el PID (número) o nombre del proceso.
Si se encuentra, se muestra la información del proceso.
4. Eliminar proceso por ID
Descripción: Elimina un proceso de la lista y libera su memoria.
Pasos:
Ingresar el PID del proceso a eliminar.
Si existe, se elimina y se libera su memoria.
5. Modificar prioridad
Descripción: Cambia la prioridad de un proceso existente.
Pasos:
Ingresar el PID del proceso.
Ingresar la nueva prioridad.
La cola de prioridad se reconstruye automáticamente.
6. Ejecutar Proceso Prioritario
Descripción: Ejecuta el proceso con mayor prioridad (menor número).
Comportamiento:
Reduce el tiempo restante del proceso en 1.
Si el tiempo restante llega a 0, el proceso se elimina y se libera su memoria.
Si aún queda tiempo, el proceso se reencola.
7. Visualizar Procesos por Prioridad
Descripción: Muestra los procesos en la cola de prioridad, ordenados por prioridad.
8. Verificar Estado de Memoria
Descripción: Muestra el estado actual de la memoria, incluyendo bloques ocupados y libres, así como la memoria disponible.
9. Guardar Datos Localmente
Descripción: Guarda todos los procesos en un archivo llamado procesos.dat para persistencia de datos.
10. Salir
Descripción: Guarda los datos y termina la ejecución del programa.
