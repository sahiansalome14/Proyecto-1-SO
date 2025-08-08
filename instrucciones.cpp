#include "instrucciones.h"
#include <fstream>
#include <iostream>
#include <regex> //activa expresiones regulares para validar cada instrucción

using namespace std;

//Valida la expresión regular recibida
bool expresionRegularInst(const string& instruccion) {
    static const regex patron(  //lo compila solo una vez
    R"(^\s*(?:(?:ADD|SUB|MUL)\s+(?:AX|BX|CX)\s*,\s*(?:AX|BX|CX|\d+)|INC\s+(?:AX|BX|CX)|JMP\s+\d+|NOP)\s*$)",
    regex::icase //ignora mayusculas y minusculas
    );


    return regex_match(instruccion, patron);
}


// Carga instrucciones desde el archivo 
vector<string> cargarInstrucciones(int pid) {
    vector<string> instruccionesValidas; 
    string nombreArchivo = to_string(pid) + ".txt";
    
    ifstream archivo(nombreArchivo); 
    if (!archivo.is_open()) {
        cerr << "⚠️ No se pudo abrir el archivo '" 
             << nombreArchivo << "' para el Proceso " << pid << "\n";
        return instruccionesValidas; 
    }

    string linea;
    int lineasInvalidas = 0;
    
    while (getline(archivo, linea)) {
        // Eliminar espacios en blanco al inicio y final
        linea.erase(0, linea.find_first_not_of(" \t"));
        linea.erase(linea.find_last_not_of(" \t") + 1);
        
        if (!linea.empty()) {
            if (expresionRegularInst(linea)) {
                instruccionesValidas.push_back(linea);
            } else {
                lineasInvalidas++;
                cerr << " Instrucción inválida ignorada (PID " 
                     << pid << "): " << linea << "\n";
            }
        }
    }
    
    if (lineasInvalidas > 0) {
        cerr << " Total instrucciones inválidas ignoradas para PID " 
             << pid << ": " << lineasInvalidas << "\n";
    }
    
    return instruccionesValidas;
}
