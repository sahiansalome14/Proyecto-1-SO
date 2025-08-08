#include "cargador.h"
#include "instrucciones.h"
#include <fstream>
#include <iostream>
#include <map>
#include <regex> //validar formato de lineas


std::map<int, Proceso> cargarProcesos(const std::string &nombreArchivo) {
    std::map<int, Proceso> procesos;
    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        std::cout << "Error: No se encontró el archivo '" << nombreArchivo << "'\n";
        return procesos;
    }

    //Para leer el archivo
    std::string linea;
    int procesosRechazados = 0; //cuantos furon rechazados

    // Patrón para capturar PID, AX, BX, CX y Quantum (AX,BX,CX son opcionales) - Valida expresión regular
    std::regex patron(
        R"(^PID\s*:\s*(\d+)(?:\s*,\s*AX\s*=\s*(\d+))?(?:\s*,\s*BX\s*=\s*(\d+))?(?:\s*,\s*CX\s*=\s*(\d+))?\s*,\s*Quantum\s*=\s*(\d+)\s*$)"
    );

    while (getline(archivo, linea)) {
        // Quitar espacios extra
        linea.erase(0, linea.find_first_not_of(" \t"));
        linea.erase(linea.find_last_not_of(" \t") + 1);

        if (linea.empty()) continue;

        std::smatch match;
        if (!std::regex_match(linea, match, patron)) {
            std::cout << " Proceso rechazado: formato inválido -> " << linea << "\n";
            procesosRechazados++;
            continue;
        }

        // Capturar valores directamente
        Proceso p{};
        p.estado = "Listo";
        p.pc = 0;
        p.pid = stoi(match[1]);
        if (match[2].matched) {
            p.ax = stoi(match[2]);
        } else {
            p.ax = 0;
        }
        if (match[3].matched) {
            p.bx = stoi(match[3]);
        } else {
            p.bx = 0;
        }

        if (match[4].matched) {
            p.cx = stoi(match[4]);
        } else {
            p.cx = 0;
        }
        p.quantum = stoi(match[5]);

        // Evitar PID duplicados
        if (procesos.find(p.pid) != procesos.end()) {
            std::cout << " Proceso con PID " << p.pid << " duplicado. Ignorado.\n";
            continue;
        }

        if (p.quantum <= 0) {
            std::cout << " Proceso " << p.pid << " descartado: Quantum inválido.\n";
            procesosRechazados++;
            continue;
        }

        // Cargar instrucciones
        p.instrucciones = cargarInstrucciones(p.pid);
        if (!p.instrucciones.empty()) {
            procesos[p.pid] = p;
        } else {
            std::cout << " Proceso " << p.pid << " descartado: sin instrucciones.\n";
            procesosRechazados++;
        }
    }

    if (procesosRechazados > 0) {
        std::cout << "Total procesos rechazados: " << procesosRechazados << "\n";
    }

    return procesos;
}
