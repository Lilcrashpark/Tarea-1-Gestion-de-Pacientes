#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definición de la estructura del paciente
typedef struct 
{
    char nombre[50];
    int edad;
    char sintoma[100];
    char prioridad[10]; // "Alto", "Medio", "Bajo"
    struct tm horaRegistro;
} Paciente;

// Definición de la estructura del nodo
typedef struct Node 
{
    Paciente paciente;
    struct Node* next;
} Node;

// Definición de la estructura de la lista de espera
typedef struct 
{
    Node* head;
    int size;
} ListaEspera;

void mostrarMenu() 
{
    printf("\n=== Sistema de Gestión Hospitalaria ===\n");
    printf("1. Registrar paciente\n");
    printf("2. Asignar prioridad a paciente\n");
    printf("3. Mostrar lista de espera\n");
    printf("4. Atender al siguiente paciente\n");
    printf("5. Mostrar pacientes por prioridad\n");
    printf("6. Salir\n");
    printf("Seleccione una opción: ");
}

ListaEspera* crearListaEspera() 
{
    ListaEspera* lista = (ListaEspera*)malloc(sizeof(ListaEspera));
    if (lista != NULL) {
        lista->head = NULL;
        lista->size = 0;
    }
    return lista;
}

void agregarPaciente(ListaEspera* lista, Paciente paciente) 
{
    Node* nuevoNodo = (Node*)malloc(sizeof(Node));
    if (nuevoNodo != NULL) 
    {
        nuevoNodo->paciente = paciente;
        nuevoNodo->next = NULL;
        if (lista->head == NULL) 
        {
            lista->head = nuevoNodo;
        } else 
        {
            Node* current = lista->head;
            while (current->next != NULL) 
            {
                current = current->next;
            }
            current->next = nuevoNodo;
        }
        lista->size++;
    }
}

void registrarPaciente(ListaEspera* lista)
{
    Paciente nuevoPaciente;
    printf("Ingrese nombre: ");
    fgets(nuevoPaciente.nombre, sizeof(nuevoPaciente.nombre), stdin);
    nuevoPaciente.nombre[strcspn(nuevoPaciente.nombre, "\n")] = '\0'; // Eliminar salto de línea
    printf("Ingrese edad: ");
    scanf("%d", &nuevoPaciente.edad);
    getchar(); // Limpiar buffer
    printf("Ingrese síntoma: ");
    fgets(nuevoPaciente.sintoma, sizeof(nuevoPaciente.sintoma), stdin);
    nuevoPaciente.sintoma[strcspn(nuevoPaciente.sintoma, "\n")] = '\0'; // Eliminar salto de línea
    strcpy(nuevoPaciente.prioridad, "Bajo"); // Prioridad inicial

    // Obtener la hora actual del sistema
    time_t tiempoActual;
    time(&tiempoActual);
    struct tm* tiempoLocal = localtime(&tiempoActual);

    // Asignar la hora actual como hora de registro
    nuevoPaciente.horaRegistro = *tiempoLocal;

    agregarPaciente(lista, nuevoPaciente);
    printf("Paciente registrado con éxito.\n");
}

void asignarPrioridad(ListaEspera* lista, char nombre[], char prioridad[]) 
{
    Node* current = lista->head;
    while (current != NULL) {
        if (strcmp(current->paciente.nombre, nombre) == 0) 
        {
            strcpy(current->paciente.prioridad, prioridad);
            printf("Prioridad asignada correctamente.\n");
            return;
        }
        current = current->next;
    }
    printf("El paciente no se encuentra en la lista de espera. No es necesario asignar prioridad.\n");
}

void asignarPrioridadSiExiste(ListaEspera* lista) 
{
    char nombre[50];
    int prioridad;
    printf("Ingrese nombre del paciente: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0'; // Eliminar salto de línea

    Node* current = lista->head;
    while (current != NULL) {
        if (strcmp(current->paciente.nombre, nombre) == 0) 
        {
            printf("Ingrese prioridad (1 - Alto, 2 - Medio, 3 - Bajo): ");
            scanf("%d", &prioridad);
            getchar(); // Limpiar buffer

            if (prioridad >= 1 && prioridad <= 3) 
            {
                char prioridadTexto[10];
                switch (prioridad) 
                {
                    case 1:
                        strcpy(prioridadTexto, "Alto");
                        break;
                    case 2:
                        strcpy(prioridadTexto, "Medio");
                        break;
                    case 3:
                        strcpy(prioridadTexto, "Bajo");
                        break;
                }
                asignarPrioridad(lista, nombre, prioridadTexto);
                return;
            } 
            else 
            {
                printf("Prioridad ingresada incorrecta. Por favor, ingrese un número del 1 al 3.\n");
                continue;
            }
        }
        current = current->next;
    }
    printf("El paciente no se encuentra en la lista de espera. Intente de nuevo.\n");
}

void mostrarListaEspera(ListaEspera* lista) 
{
    printf("\n=== Lista de espera ===\n");

    // Mostrar los pacientes en el orden de llegada
    Node* current = lista->head;
    while (current != NULL) 
    {
        printf("Nombre: %s, Edad: %d, Síntoma: %s, Prioridad: %s, Hora de Registro: %02d:%02d\n",
            current->paciente.nombre, current->paciente.edad, current->paciente.sintoma,
            current->paciente.prioridad, current->paciente.horaRegistro.tm_hour, current->paciente.horaRegistro.tm_min);
        current = current->next;
    }
}

void atenderSiguientePaciente(ListaEspera* lista) 
{
    if (lista->head == NULL) 
    {
        printf("No hay pacientes en espera.\n");
        return;
    }

    // Buscar paciente con la mayor prioridad y llegada más temprana
    Node* previous = NULL;
    Node* current = lista->head;
    Node* maxPriorityNode = current;
    Node* previousMaxPriorityNode = NULL;

    while (current != NULL) 
    {
        if ((strcmp(current->paciente.prioridad, "Alto") == 0 && 
             strcmp(maxPriorityNode->paciente.prioridad, "Alto") != 0) ||
            (strcmp(current->paciente.prioridad, "Medio") == 0 && 
             strcmp(maxPriorityNode->paciente.prioridad, "Alto") != 0 &&
             strcmp(maxPriorityNode->paciente.prioridad, "Medio") != 0) ||
            (strcmp(current->paciente.prioridad, "Bajo") == 0 &&
             strcmp(maxPriorityNode->paciente.prioridad, "Alto") != 0 &&
             strcmp(maxPriorityNode->paciente.prioridad, "Medio") != 0))
        {
            maxPriorityNode = current;
            previousMaxPriorityNode = previous;
        }

        previous = current;
        current = current->next;
    }

    // Atender al paciente encontrado
    if (previousMaxPriorityNode == NULL) 
    {
        lista->head = maxPriorityNode->next;
    } 
    else 
    {
        previousMaxPriorityNode->next = maxPriorityNode->next;
    }

    printf("\n=== Paciente atendido ===\n");
    printf("Nombre: %s, Edad: %d, Síntoma: %s, Prioridad: %s, Hora de Registro: %02d:%02d\n",
        maxPriorityNode->paciente.nombre, maxPriorityNode->paciente.edad, maxPriorityNode->paciente.sintoma,
        maxPriorityNode->paciente.prioridad, maxPriorityNode->paciente.horaRegistro.tm_hour, maxPriorityNode->paciente.horaRegistro.tm_min);
    free(maxPriorityNode);
    lista->size--;
}


void mostrarPacientesPorPrioridad(ListaEspera* lista) 
{
    int prioridad;
    do {
        printf("\n=== Mostrar pacientes por prioridad ===\n");
        printf("Ingrese prioridad (1 - Alto, 2 - Medio, 3 - Bajo): ");
        scanf("%d", &prioridad);
        getchar(); // Limpiar buffer

        if (prioridad < 1 || prioridad > 3) 
        {
            printf("Prioridad ingresada incorrecta. Por favor, ingrese un número del 1 al 3.\n");
        }
    } while (prioridad < 1 || prioridad > 3);

    char prioridadTexto[10];
    switch (prioridad) 
    {
        case 1:
            strcpy(prioridadTexto, "Alto");
            break;
        case 2:
            strcpy(prioridadTexto, "Medio");
            break;
        case 3:
            strcpy(prioridadTexto, "Bajo");
            break;
    }

    printf("\n=== Pacientes con prioridad %s ===\n", prioridadTexto);
    Node* current = lista->head;
    while (current != NULL) 
    {
        if (strcmp(current->paciente.prioridad, prioridadTexto) == 0) 
        {
            printf("Nombre: %s, Edad: %d, Síntoma: %s, Hora de Registro: %02d:%02d\n",
                current->paciente.nombre, current->paciente.edad, current->paciente.sintoma,
                current->paciente.horaRegistro.tm_hour, current->paciente.horaRegistro.tm_min);
        }
        current = current->next;
    }
}

int main() 
{
    ListaEspera* lista = crearListaEspera();
    int opcion;
    do {
        mostrarMenu();
        scanf("%d", &opcion);
        getchar(); // Limpiar buffer
        switch (opcion) 
        {
            case 1: 
            {
                // Registro de paciente
                registrarPaciente(lista);
                break;
            }
            case 2: 
            {
                // Asignar prioridad a paciente si existe en la lista
                asignarPrioridadSiExiste(lista);
                break;
            }
            case 3: 
            {
                // Mostrar lista de espera
                mostrarListaEspera(lista);
                break;
            }
            case 4: 
            {
                // Atender al siguiente paciente 
                atenderSiguientePaciente(lista);
                break;
            }
            case 5: 
            {
                // Mostrar pacientes por prioridad
                mostrarPacientesPorPrioridad(lista);
                break;
            }
            case 6: 
            {
                // Salir y liberar memoria
                printf("Saliendo del sistema.\n");
                // Liberar memoria de la lista de espera
                Node* current = lista->head;
                Node* temp;
                while (current != NULL) 
                {
                    temp = current;
                    current = current->next;
                    free(temp);
                }
                free(lista);
                break;
            }
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }

    } while (opcion != 6);

    return 0;
}
