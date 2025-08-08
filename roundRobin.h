#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include "proceso.h"
#include <map>
#include <string>

std::string convertirMayusculas(const std::string& texto);

bool procesarInstruccion(Proceso& proceso, const std::string& lineaInstruccion);
void mostrarTablaContexto(const std::map<int, Proceso>& procesos, std ::ofstream* flog = nullptr);
void ejecutarRoundRobin(std::map<int, Proceso>& listaProcesos);

#endif

