#include "cargador.h"
#include "roundRobin.h"
#include <iostream>
#include <iomanip>
#include <map>  
using namespace std;

// Función para mostrar los procesos en tabla
void mostrarProcesosEnTabla(const map<int, Proceso>& procesos) {
    cout << "╔══════╦══════╦══════╦══════╦══════╦══════════╦════════════╗\n";
    cout << "║ PID  ║  PC  ║  AX  ║  BX  ║  CX  ║  Quantum ║   Estado   ║\n";
    cout << "╠══════╬══════╬══════╬══════╬══════╬══════════╬════════════╣\n";

    for (const auto& par : procesos) {
        const Proceso& p = par.second;
        cout << "║ " << setw(4) << right << p.pid
             << " ║ " << setw(4) << right << p.pc
             << " ║ " << setw(4) << right << p.ax
             << " ║ " << setw(4) << right << p.bx
             << " ║ " << setw(4) << right << p.cx
             << " ║ " << setw(8) << right << p.quantum
             << " ║ " << setw(10) << left  << p.estado
             << " ║\n";
    }

    cout << "╚══════╩══════╩══════╩══════╩══════╩══════════╩════════════╝\n";
}

// Función para mostrar instrucciones de cada proceso
// Función para mostrar instrucciones de cada proceso con índice
void mostrarInstrucciones(const map<int, Proceso>& procesos) {
    for (const auto& par : procesos) {
        const Proceso& p = par.second;
        cout << "\n Instrucciones del PID " << p.pid << ":\n";

        if (!p.instrucciones.empty()) {
            for (size_t i = 0; i < p.instrucciones.size(); ++i) {
                cout << " (" << i << ") " << p.instrucciones[i] << "\n";
            }
        } else {
            cout << " Sin instrucciones cargadas.\n";
        }
    }
}


int main() {
    cout << "Ingrese nombre del archivo de procesos: ";
    string archivo;
    cin >> archivo;

    auto procesos = cargarProcesos(archivo);  

    cout << "\n Procesos Cargados (" << procesos.size() << ")\n";

    if (!procesos.empty()) {
        mostrarProcesosEnTabla(procesos);
        mostrarInstrucciones(procesos);

        ejecutarRoundRobin(procesos);

        cout << "\n Estado final de los procesos:\n";
        mostrarProcesosEnTabla(procesos);

    } else {
        cout << " No se cargaron procesos válidos.\n";
    }

    return 0;
}
