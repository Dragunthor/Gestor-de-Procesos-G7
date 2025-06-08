#include <iostream>
#include <cstring>

using namespace std;

// Estructura de un proceso
struct Proceso {
    int pid;
    char nombre[32];
    int prioridad;
    int memRequerida;
};

// Nodo para la lista enlazada de procesos
struct NodoProceso {
    Proceso* dato;
    NodoProceso* siguiente;
};

// Nodo para la cola de prioridad
struct NodoCola {
    Proceso* dato;
    NodoCola* siguiente;
};

//Estructura de Memoria
struct BloqueMemoria {
    Proceso* proceso;
    int inicio;
    int tamano;
    bool libre;
};

struct NodoMemoria {
    BloqueMemoria dato;
    NodoMemoria* siguiente;
};

NodoMemoria* pilaMemoria = nullptr;
const int MEMORIA_TOTAL = 4096;  // 4KB de memoria total
int memoriaDisponible = MEMORIA_TOTAL;

// Función para detectar si una cadena representa un número entero
bool esNumero(const char* s) {
    if (s == nullptr || *s == '\0') return false;
    int i = 0;
    if (s[0] == '-' && s[1] != '\0') i = 1;
    for (; s[i] != '\0'; ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

// Crear un nuevo nodo usando new
NodoProceso* crearNodo(Proceso* p) {
    return new NodoProceso{p, nullptr};
}

// Insertar un nuevo proceso: recibe valores y construye el Proceso internamente
Proceso* insertarProceso(NodoProceso*& cabeza,
                     int pid,
                     const char* nombreEventos,
                     int prioridad,
                     int memReq) {
    // Crear Proceso en heap
    Proceso* p = new Proceso;
    p->pid = pid;
    // Copiar el nombre (asegurarse de truncar)
    strncpy(p->nombre, nombreEventos, sizeof(p->nombre) - 1);
    p->nombre[sizeof(p->nombre) - 1] = '\0';
    p->prioridad = prioridad;
    p->memRequerida = memReq;

    // Insertar en lista enlazada
    NodoProceso* nodo = crearNodo(p);
    if (!cabeza) {
        cabeza = nodo;
    } else {
        NodoProceso* it = cabeza;
        while (it->siguiente) it = it->siguiente;
        it->siguiente = nodo;
    }
    return p;
}

// Buscar proceso por ID o por nombre
NodoProceso* buscarProceso(NodoProceso* cabeza, const char* entrada) {
    bool numero = esNumero(entrada);
    NodoProceso* it = cabeza;
    while (it) {
        if (numero && it->dato->pid == stoi(entrada)) {
            return it;
        } else if (!numero && strcmp(it->dato->nombre, entrada) == 0) {
            return it;
        }
        it = it->siguiente;
    }
    return nullptr;
}
//Eliminar proceso por puntero
bool eliminarProcesoDeLista(NodoProceso*& cabeza, Proceso* procAEliminar) {
    if (!cabeza || !procAEliminar) return false;

    NodoProceso* ant = nullptr;
    NodoProceso* it = cabeza;
    
    while (it && it->dato != procAEliminar) {
        ant = it;
        it = it->siguiente;
    }
    
    if (!it) return false; // No se encontró el proceso
    
    // Reorganizar los nodos
    if (!ant) cabeza = it->siguiente;
    else ant->siguiente = it->siguiente;
    
    delete it;
    return true;
}

// Modificar prioridad de un proceso
bool modificarPrioridad(NodoProceso* cabeza, int pid, int nuevaPrioridad) {
    NodoProceso* it = cabeza;
    while (it) {
        if (it->dato->pid == pid) {
            it->dato->prioridad = nuevaPrioridad;
            return true;
        }
        it = it->siguiente;
    }
    return false;
}

// Mostrar todos los procesos registrados
void mostrarProcesos(NodoProceso* cabeza) {
    cout << "PID\tNombre\t\tPrioridad\tMemoria" << endl;
    NodoProceso* it = cabeza;
    while (it) {
        Proceso* p = it->dato;
        cout << p->pid << "\t" << p->nombre
             << "\t\t" << p->prioridad
             << "\t\t" << p->memRequerida << endl;
        it = it->siguiente;
    }
}

// Liberar toda la lista enlazada
void liberarLista(NodoProceso*& cabeza) {
    while (cabeza) {
        NodoProceso* sig = cabeza->siguiente;
        delete cabeza->dato;
        delete cabeza;
        cabeza = sig;
    }
}

// ENCOLAR según prioridad (lista ordenada creciente)
NodoCola* encolarPrioridad(NodoCola*& frente, Proceso* p) {
    NodoCola* nodo = new NodoCola{p,nullptr};
    if (!frente || p->prioridad < frente->dato->prioridad) {
        nodo->siguiente = frente; frente = nodo;
    } else {
        NodoCola* it = frente;
        while (it->siguiente && it->siguiente->dato->prioridad <= p->prioridad)
            it = it->siguiente;
        nodo->siguiente = it->siguiente;
        it->siguiente = nodo;
    }
    return frente;
}

// DESENCOLAR: extraer frente
Proceso* desencolarPrioridad(NodoCola*& frente, NodoProceso*& lista) {
    if (!frente) return nullptr;
    
    Proceso* p = frente->dato;
    NodoCola* aux = frente;
    frente = frente->siguiente;
    delete aux;

    if (!eliminarProcesoDeLista(lista, p)) {
        cout << "Advertencia: Proceso no encontrado en lista principal\n";
    }

    return p;
}

// Mostrar cola de prioridad
void mostrarCola(NodoCola* frente) {
    cout<<"PID\tNombre\t\tPrioridad"<<endl;
    for (NodoCola* it=frente; it; it=it->siguiente) {
        Proceso* p=it->dato;
        cout<<p->pid<<"\t"<<p->nombre<<"\t\t"<<p->prioridad<<endl;
    }
}

//Liberar Cola de Prioridad
void liberarCola(NodoCola*& frente) {
    while (frente) {
        NodoCola* sig=frente->siguiente;
        // no borramos el Proceso, queda en lista principal
        delete frente;
        frente=sig;
    }
}

// Actualizar Cola
void reconstruirCola(NodoCola*& frente, NodoProceso* lista) {
    // Liberar la cola actual (sin borrar los procesos, solo los nodos)
    liberarCola(frente);
    
    // Volver a encolar todos los procesos de la lista
    NodoProceso* it = lista;
    while (it) {
        Proceso* p = new Proceso; // Crear una copia para la cola
        *p = *(it->dato);         // Copiar los datos del proceso
        encolarPrioridad(frente, p);
        it = it->siguiente;
    }
}

//Asignar Memoria
void asignarMemoria(Proceso* p) {
    // Calcular inicio como espacio ocupado: total - disponible
    int inicio = MEMORIA_TOTAL - memoriaDisponible;
    NodoMemoria* nodo = new NodoMemoria{{p, inicio, p->memRequerida, false}, nullptr};
    nodo->dato.libre = false;
    nodo->siguiente = pilaMemoria;
    pilaMemoria = nodo;
    memoriaDisponible -= p->memRequerida;
}

//Liberar Memoria
void liberarMemoria(Proceso* p) {
    for (NodoMemoria* it = pilaMemoria; it; it = it->siguiente) {
        if (it->dato.proceso == p && !it->dato.libre) {
            it->dato.libre = true;
            memoriaDisponible += it->dato.tamano;
            return;
        }
    }
}

//Mostrar el estado de la Memoria
void mostrarEstadoMemoria() {
    cout << "--- Estado de Memoria ---" << endl;
    cout << "Proceso	Inicio	Tamano	Estado" << endl;
    for (NodoMemoria* it = pilaMemoria; it; it = it->siguiente) {
        auto& b = it->dato;
        cout << b.proceso->nombre << "	" << b.inicio << "	" << b.tamano << "	"
             << (b.libre ? "Libre" : "Ocupado") << endl;
    }
    cout << "Memoria disponible: " << memoriaDisponible << " / " << MEMORIA_TOTAL << endl;
}


// Eliminar proceso por PID
bool eliminarPorPID(NodoProceso*& cabeza, NodoCola*& cola, int pid) {
    // Buscar el proceso en la lista principal
    NodoProceso* it = cabeza;
    while (it && it->dato->pid != pid) {
        it = it->siguiente;
    }
    if (!it) return false;

    Proceso* procAEliminar = it->dato;

    // Eliminar de la lista principal (la función maneja los enlaces internos)
    eliminarProcesoDeLista(cabeza, procAEliminar);

    // Reconstruir la cola completa
    reconstruirCola(cola, cabeza);

    // Liberar memoria
    delete procAEliminar;
    return true;
}

// Menú de consola con protección de ingreso y detección de tipo
int main() {
    NodoProceso* lista = nullptr;
    NodoCola* cola=nullptr;
    int opcion;
    char entrada[32];

    do {
        cout << "\n--- GESTOR DE PROCESOS ---" << endl;
        cout << "1. Insertar nuevo proceso" << endl;
        cout << "2. Mostrar todos los procesos" << endl;
        cout << "3. Buscar proceso (por ID o nombre)" << endl;
        cout << "4. Eliminar proceso por ID" << endl;
        cout << "5. Modificar prioridad" << endl;
        cout << "6. Ejecutar Proceso Prioritario"<<endl;
        cout << "7. Visualizar Procesos por Prioridad"<<endl;
        cout << "8. Verificar Estado de Memoria"<<endl;
        cout << "9. Salir"<<endl;
        cout <<"Opcion: ";
        cin.getline(entrada, sizeof(entrada));

        opcion = (esNumero(entrada) ? stoi(entrada) : -1);

        switch (opcion) {
            case 1: {
                // Pedir datos con validación
                cout << "Ingrese ID (entero): ";
                cin.getline(entrada, sizeof(entrada));
                if (!esNumero(entrada)) { cout << "ID no válido.\n"; break; }
                int pid = stoi(entrada);

                cout << "Ingrese nombre: ";
                char nombre[32];
                cin.getline(nombre, sizeof(nombre));

                cout << "Ingrese prioridad (0 = alta): ";
                cin.getline(entrada, sizeof(entrada));
                if (!esNumero(entrada)) { cout << "Prioridad no válida.\n"; break; }
                int prioridad = stoi(entrada);

                cout << "Ingrese memoria requerida: ";
                cin.getline(entrada, sizeof(entrada));
                if (!esNumero(entrada)) { cout << "Memoria no válida.\n"; break; }
                int memoria = stoi(entrada);

                if (memoria > memoriaDisponible) {
                    cout << "Error: No hay suficiente memoria disponible (" 
                        << memoriaDisponible << " bytes libres de " << MEMORIA_TOTAL << ")\n";
                    break;
                }

                Proceso* nuevoProceso= insertarProceso(lista, pid, nombre, prioridad, memoria);
    
                encolarPrioridad(cola, nuevoProceso);
                asignarMemoria(nuevoProceso);
                cout << "Proceso agregado.\n";
                break;
            }
            case 2:
                mostrarProcesos(lista);
                break;
            case 3: {
                cout << "Ingrese ID o nombre: ";
                cin.getline(entrada, sizeof(entrada));
                NodoProceso* encontrado = buscarProceso(lista, entrada);
                if (encontrado) {
                    Proceso* p = encontrado->dato;
                    cout << "Encontrado: PID " << p->pid
                         << ", Nombre: " << p->nombre
                         << ", Prioridad: " << p->prioridad
                         << ", Memoria: " << p->memRequerida << endl;
                } else cout << "No se encontró el proceso.\n";
                break;
            }
            case 4: {
                cout << "Ingrese PID a eliminar: ";
                cin.getline(entrada, sizeof(entrada));
                if (!esNumero(entrada)) { cout << "PID no válido.\n"; break; }
                int pid = stoi(entrada);
                NodoProceso* nodo = buscarProceso(lista, entrada);
                if (nodo) liberarMemoria(nodo->dato);
                cout << (eliminarPorPID(lista, cola, pid) ? "Proceso eliminado.\n" : "No se encontró el proceso.\n");
                break;
            }
            case 5: {
                cout << "Ingrese PID a modificar: ";
                cin.getline(entrada, sizeof(entrada));
                if (!esNumero(entrada)) { cout << "PID no válido.\n"; break; }
                int pid = stoi(entrada);

                cout << "Nueva prioridad: ";
                cin.getline(entrada, sizeof(entrada));
                if (!esNumero(entrada)) { cout << "Prioridad no válida.\n"; break; }
                int nueva = stoi(entrada);

                cout << (modificarPrioridad(lista, pid, nueva) ? "Prioridad actualizada.\n" : "No se encontró el proceso.\n");
                break;
            }
            case 6: {
                Proceso* p = desencolarPrioridad(cola,lista);
                if (p){
                    liberarMemoria(p);
                    cout<<"Ejecutando PID "<<p->pid<<" N:"<<p->nombre<<"\n";}
                else cout<<"Cola vacía\n";
                break; }
            case 7: 
                mostrarCola(cola); 
                break;
            case 8:
                mostrarEstadoMemoria();
                break;
            case 9: 
                liberarCola(cola); 
                liberarLista(lista); 
                cout<<"Saliendo\n"; 
                break;
            default: cout<<"Opcion invalida\n";
        }
    } while (opcion != 9);

    return 0;
}
