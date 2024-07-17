#include <stdio.h>     // Librería para la entrada y salida estándar
#include <vector>       // Librería para utilizar el contenedor vector
#include <ctime>        // Librería que nos ayudara a trabajar con funciones de tiempo
#include <cstdlib>      // Librería para funciones generales de propósito, esto inclye la generación de números aleatorios
#include <algorithm>    // Librería para algoritmos estándar 
#include <random>       // Librería para generar números aleatorios

using namespace std;    

int TAMANIO;            // Variable para almacenar el tamaño del laberinto
char **laberinto;       // Puntero doble para la matriz del laberinto
bool **visitado;        // Puntero doble para la matriz de celdas visitadas
int pasos = 0;          // Contador de pasos para resolver el laberinto

// Se inicia con una función para inicializar el laberinto y la matriz de celdas visitadas
void inicializarLaberinto(int tamanio) {
    laberinto = new char*[tamanio];          // se le asigna memoria para las filas del laberinto
    visitado = new bool*[tamanio];           // se le asigna memoria para las filas de la matriz visitada
    for (int i = 0; i < tamanio; ++i) {      // Y se recorre sobre cada fila
        laberinto[i] = new char[tamanio];    // se le asigna memoria para las columnas del laberinto
        visitado[i] = new bool[tamanio];     // se le asigna memoria para las columnas de la matriz visitada
        for (int j = 0; j < tamanio; ++j) {  // y se recorre sobre cada columna
            laberinto[i][j] = '#';           // Iniciamos todas las celdas como paredes
            visitado[i][j] = false;          // Iniciamos todas las celdas como no visitadas
        }
    }
}

// luego otra función para liberar la memoria del laberinto y la matriz de celdas visitadas
void liberarLaberinto(int tamanio) {
    for (int i = 0; i < tamanio; ++i) {      // recorremos sobre cada fila
        delete[] laberinto[i];               // liberamos memoria para las columnas del laberinto
        delete[] visitado[i];                // Liberamos memoria para las columnas de la matriz visitada
    }
    delete[] laberinto;                      // Liberamos memoria para las filas del laberinto
    delete[] visitado;                       // Liberamos memoria para las filas de la matriz visitada
}                                            // LIBERAMOS memorio nos ayudara a generar los laberitos aleatorios cada ves que iniciamos uno este no imprimira el mismo 

// esta función nos ayudara a mostrar el laberinto en la consola
void mostrarLaberinto(int tamanio) {
    for (int i = 0; i < tamanio; ++i) {      // Recorremos sobre cada fila
        for (int j = 0; j < tamanio; ++j) {  // Recorremos sobre cada columna
            printf("%c ", laberinto[i][j]);  // Imprimir cada celda del laberinto
        }
        printf("\n");                        // Imprimir un salto de línea al final de cada fila
    }
}

// esta funcion sierve para verificar si una celda es válida y esta dentro de los límites y no visitada
bool esValido(int x, int y, int tamanio) {
    return x >= 0 && x < tamanio && y >= 0 && y < tamanio && !visitado[x][y];  // se retornara verdadero si la celda está dentro de los límites y no ha sido visitada
}

// Función para generar el laberinto 
void generarLaberinto(int x, int y, int tamanio) {
    visitado[x][y] = true;                   // se marcara la celda actual como visitada
    laberinto[x][y] = ' ';                   // se marcara la celda actual como camino

    int direcciones[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // esta son las direccines para poder moverse 
    random_device rd;                        
    mt19937 g(rd());                         // aqui es un generador de números aleatorios
    shuffle(begin(direcciones), end(direcciones), g);  // se mesclaran las direcciones 

    for (auto dir : direcciones) {           // Intentar moverse en cada dirección
        int nx = x + dir[0] * 2, ny = y + dir[1] * 2;  // Calcular la nueva posición
        if (esValido(nx, ny, tamanio)) {     // Verificar si la nueva posición es válida
            laberinto[x + dir[0]][y + dir[1]] = ' ';  // Crear camino intermedio
            generarLaberinto(nx, ny, tamanio);  
        }
    }
}

// en esta función recursiva se resolver el laberinto
bool resolverLaberinto(int x, int y, int tamanio) {
    if (x == tamanio - 2 && y == tamanio - 2) {  // Si llegamos a la salida
        laberinto[x][y] = 'S';                   // Marcar la salida
        return true;                             // Retornar verdadero
    }

    if (laberinto[x][y] == ' ' || laberinto[x][y] == 'E') {  // Verificar si la celda es un camino entrada
        laberinto[x][y] = '.';                    // Marcar la celda como visitada

        int direcciones[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // Direcciones para moverse 
        for (auto dir : direcciones) {           // Intentar moverse en cada dirección
            int nx = x + dir[0], ny = y + dir[1]; // Calcular la nueva posición
            if (resolverLaberinto(nx, ny, tamanio)) {  
                pasos++;                           // Incrementar el contador de pasos
                return true;                       // Retornar verdadero
            }
        }
        laberinto[x][y] = ' ';                    // Desmarcar la celda si no se encuentra la salida
    }
    return false;                                // Retornar falso si no se encuentra la salida
}

int main() {
    srand(time(0));                             

    do {
        TAMANIO = 13 + rand() % 13;              // Generar tamaño aleatorio 
    } while (TAMANIO % 2 == 0);                  

    inicializarLaberinto(TAMANIO);               // Inicializar el laberinto
    generarLaberinto(1, 1, TAMANIO);             // Generar laberinto desde (1,1)
    laberinto[1][1] = 'E';                       // Marcar la entrada
    laberinto[TAMANIO - 2][TAMANIO - 2] = ' ';   // Marcar la salida

    printf("Laberinto generado (Tamaño: %d):\n", TAMANIO);  // Imprimir el tamaño del laberinto generado
    mostrarLaberinto(TAMANIO);                   // Mostrar el laberinto generado

    pasos = 0;                                   // Reiniciar el contador de pasos
    if (resolverLaberinto(1, 1, TAMANIO)) {      // Intentar resolver el laberinto desde la entrada
        printf("\nLaberinto resuelto:\n");       // Imprimir mensaje si se resuelve el laberinto
        mostrarLaberinto(TAMANIO);               // Mostrar el laberinto resuelto
        printf("\nPasos necesarios para resolver el laberinto: %d\n", pasos);  // Imprimir el número de pasos necesarios
    } else {
        printf("No se pudo encontrar una salida.\n");  // Imprimir mensaje si no se encuentra una salida
    }

    liberarLaberinto(TAMANIO);                   

    return 0;                                    
}
