#ifndef PROCESO_H
#define PROCESO_H

#include <string>
#include <vector>

using namespace std; 

struct Proceso {
    int pid;        // ID del proceso
    int pc;         // Contador de programa
    int ax, bx, cx; // Registros
    int quantum;    // Tiempo asignado
    string estado;  // Estado actual
    vector<string> instrucciones; // Lista de instrucciones a ejecutar
    int tiempoBloqueo; //contador de "ticks" que le falta al proceso para desbloquearse
    int repeticionesMaxJMP[50] = {0};  // Guarda cuántas veces se saltó a cada línea



    // Constructor: se inicializa todos los campos
    Proceso() : pid(0), pc(0), ax(0), bx(0), cx(0), quantum(0), estado("Listo"), tiempoBloqueo(0) {}
};
#endif
