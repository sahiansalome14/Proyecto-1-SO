#include "roundRobin.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <map>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <fstream> // Para el archivo .log

using namespace std;

static inline string corta(const string& s, size_t w) {
    return (s.size()>w) ? s.substr(0, w) : s;
}

string convertirMayusculas(const string& texto) {
    string resultado = texto;
    for (int i = 0; i < resultado.size(); i++) {
        resultado[i] = toupper(resultado[i]);
    }
    return resultado;
}

string procesarInstruccion(Proceso& proceso, const std::string& lineaInstruccion, bool& avanzarPC) {
    istringstream flujoTexto(lineaInstruccion);
    string operacion;
    flujoTexto >> operacion;
    operacion = convertirMayusculas(operacion);

    string primerParametro, segundoParametro;
    string restoLinea;
    getline(flujoTexto, restoLinea);

    restoLinea.erase(remove(restoLinea.begin(), restoLinea.end(), ','), restoLinea.end());
    istringstream flujoParametros(restoLinea);
    flujoParametros >> primerParametro >> segundoParametro;

    string resultadoOperacion = "";
    avanzarPC = true;

    if (operacion == "ADD" || operacion == "SUB" || operacion == "MUL") {
        int valor = 0;

        if (convertirMayusculas(segundoParametro) == "AX") valor = proceso.ax;
        else if (convertirMayusculas(segundoParametro) == "BX") valor = proceso.bx;
        else if (convertirMayusculas(segundoParametro) == "CX") valor = proceso.cx;
        else if (!segundoParametro.empty()) valor = stoi(segundoParametro);

        string reg = convertirMayusculas(primerParametro);
        int antes = 0;
        int despues = 0;

        if (reg == "AX") {
            antes = proceso.ax;
            if (operacion == "ADD") proceso.ax += valor;
            else if (operacion == "SUB") proceso.ax -= valor;
            else if (operacion == "MUL") proceso.ax *= valor;
            despues = proceso.ax;
        } 
        else if (reg == "BX") {
            antes = proceso.bx;
            if (operacion == "ADD") proceso.bx += valor;
            else if (operacion == "SUB") proceso.bx -= valor;
            else if (operacion == "MUL") proceso.bx *= valor;
            despues = proceso.bx;
        } 
        else if (reg == "CX") {
            antes = proceso.cx;
            if (operacion == "ADD") proceso.cx += valor;
            else if (operacion == "SUB") proceso.cx -= valor;
            else if (operacion == "MUL") proceso.cx *= valor;
            despues = proceso.cx;
        }

        resultadoOperacion = " Resultado: " + reg + " = " + to_string(antes) + " " + operacion + " " + to_string(valor) + " = " + to_string(despues);
    }
    else if (operacion == "INC") {
        string reg = convertirMayusculas(primerParametro);
        int antes = 0, despues = 0;

        if (reg == "AX") {
            antes = proceso.ax;
            proceso.ax++;
            despues = proceso.ax;
            resultadoOperacion = " Resultado: AX = " + to_string(antes) + " + 1 = " + to_string(despues);
        } 
        else if (reg == "BX") {
            antes = proceso.bx;
            proceso.bx++;
            despues = proceso.bx;
            resultadoOperacion = " Resultado: BX = " + to_string(antes) + " + 1 = " + to_string(despues);
        } 
        else if (reg == "CX") {
            antes = proceso.cx;
            proceso.cx++;
            despues = proceso.cx;
            resultadoOperacion = " Resultado: CX = " + to_string(antes) + " + 1 = " + to_string(despues);
        }
    }
    else if (operacion == "JMP") {
        int lineaSalto = stoi(primerParametro);

        if (lineaSalto >= 0 && lineaSalto < (int)proceso.instrucciones.size()) {
            proceso.repeticionesMaxJMP[lineaSalto]++;
            if (proceso.repeticionesMaxJMP[lineaSalto] <= 2) {
                proceso.pc = lineaSalto;
                avanzarPC = false;
                return " Saltando a la línea " + to_string(lineaSalto);
            } 
            else {
                resultadoOperacion = "⚠️ Límite de 2 saltos a línea " + to_string(lineaSalto) + " alcanzado.";
            }
        } 
        else {
            resultadoOperacion = "⚠️ Salto inválido a línea " + to_string(lineaSalto);
        }
    }
    else if (operacion == "NOP") {
        // No operation
    }

    return resultadoOperacion;
}

void mostrarTablaContexto(const map<int, Proceso>& procesos, std :: ofstream* flog) {
    cout << "\n Cambio de contexto...\n";
    if (flog) (*flog) << "\nCambio de Contexto...\n";

    cout << "\n Estado actual de los procesos:\n";
    if (flog) (*flog) << "\nEstado actual de los procesos:\n";
    cout << "╔══════╦══════╦══════╦══════╦══════╦══════════╦══════════════╗\n";
    cout << "║ PID  ║  PC  ║  AX  ║  BX  ║  CX  ║  Quantum ║   Estado     ║\n";
    cout << "╠══════╬══════╬══════╬══════╬══════╬══════════╬══════════════╣\n";

    const int ANCHO_ESTADO = 12;

    for (const auto& par : procesos) {
        const Proceso& p = par.second;

        // Mostrar "Bloqueado(n)" cuando aplica
        string estadoMostrar = p.estado;
        if (p.estado == "Bloqueado") {
            estadoMostrar += "(" + to_string(p.tiempoBloqueo) + ")";
        }

        cout << "║ " << setw(4) << right << p.pid
             << " ║ " << setw(4) << right << p.pc
             << " ║ " << setw(4) << right << p.ax
             << " ║ " << setw(4) << right << p.bx
             << " ║ " << setw(4) << right << p.cx
             << " ║ " << setw(8) << right << p.quantum
             << " ║ " << setw(ANCHO_ESTADO) << left  << corta(estadoMostrar, ANCHO_ESTADO)
             << " ║\n";
    }

    cout << "╚══════╩══════╩══════╩══════╩══════╩══════════╩══════════════╝\n";
}


void ejecutarRoundRobin(map<int, Proceso>& listaProcesos) {
    std::ofstream log("Seguimiento.log"); //crea o sobreescribe el archivo
    queue<int> colaEjecucion;
    srand(time(0)); 

    for (auto& elemento : listaProcesos) {
        colaEjecucion.push(elemento.first);
    }

    cout << "\nIniciando ejecución con Round Robin...\n";
    bool limpiadoBuffer = false; // para limpiar el \n solo una vez

    while (!colaEjecucion.empty()) {
        int idProceso = colaEjecucion.front();
        colaEjecucion.pop();

        Proceso& procesoActual = listaProcesos[idProceso];

        if(procesoActual.estado == "Bloqueado"){
            procesoActual.tiempoBloqueo--;
            if(procesoActual.tiempoBloqueo > 0){
                colaEjecucion.push(idProceso);
                mostrarTablaContexto(listaProcesos, &log);
                continue;
            } 
            //Se desbloquea
            procesoActual.estado = "Listo";
        }


        procesoActual.estado = "Ejecutando";
        
        log <<"\nProceso PID: " << procesoActual.pid
             << " (Quantum = " << procesoActual.quantum
             << ", Estado: " << procesoActual.estado << ")\n";

        cout <<"\nProceso PID: " << procesoActual.pid
             << " (Quantum = " << procesoActual.quantum
             << ", Estado: " << procesoActual.estado << ")\n";

        int instruccionesEjecutadas = 0;

        while (instruccionesEjecutadas < procesoActual.quantum &&
               procesoActual.pc < (int)procesoActual.instrucciones.size()) {

            // --- Pausa paso a paso: limpiar buffer la primera vez ---
            if (!limpiadoBuffer) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                limpiadoBuffer = true;
            }

            // Mostrar estado ANTES
            log << "Estado ANTES: AX = " << procesoActual.ax
                 << ", BX = " << procesoActual.bx
                 << ", CX = " << procesoActual.cx << "\n";

            // Mostrar instrucción actual
            log << "Ejecutando instrucción (" << procesoActual.pc << "): "
                 << procesoActual.instrucciones[procesoActual.pc] << "\n";
            
            cout << "Estado ANTES: AX = " << procesoActual.ax
                 << ", BX = " << procesoActual.bx
                 << ", CX = " << procesoActual.cx << "\n";

            // Mostrar instrucción actual
            cout << "Ejecutando instrucción (" << procesoActual.pc << "): "
                 << procesoActual.instrucciones[procesoActual.pc] << "\n";


            // Pausa
            cout << "Presiona ENTER para continuar...\n";
            cin.get();

            // Ejecutar la instrucción
            bool avanzar;
            string resultado = procesarInstruccion(
                procesoActual,
                procesoActual.instrucciones[procesoActual.pc],
                avanzar
            );

            if (!resultado.empty()) log <<resultado << "\n";

            if (avanzar) {
                procesoActual.pc++;
            }

            // Mostrar estado DESPUÉS
            log << "Estado DESPUÉS de ejecutar:\n";
            log  << "AX = " << procesoActual.ax
                 << ", BX = " << procesoActual.bx
                 << ", CX = " << procesoActual.cx << "\n";
            log << "----------------------------------------------\n";

            cout << "Estado DESPUÉS de ejecutar:\n";
            cout << "AX = " << procesoActual.ax
                 << ", BX = " << procesoActual.bx
                 << ", CX = " << procesoActual.cx << "\n";
            cout << "----------------------------------------------\n";

            instruccionesEjecutadas++;

            if(instruccionesEjecutadas < procesoActual.quantum && (rand() % 100) < 30){
                procesoActual.estado = "Bloqueado";
                procesoActual.tiempoBloqueo = 2;
                cout<<"Proceso "<< procesoActual.pid << " BLOQUEADO antes de agotar el quantum.\n";
                break;
            }
        }

        if (procesoActual.pc >= (int)procesoActual.instrucciones.size()) {
            procesoActual.estado = "Terminado";
            cout << " Proceso " << procesoActual.pid << " finalizado.\n";
        } else if (procesoActual.estado == "Bloqueado"){
            colaEjecucion.push(procesoActual.pid);
        }
        else {
            procesoActual.estado = "Listo";
            colaEjecucion.push(procesoActual.pid);
        }

        mostrarTablaContexto(listaProcesos, &log);
    }

    cout << "\n Todos los procesos han finalizado.\n";
    log << "\n Todos los procesos han finalizado.\n";
    log.close();
}

