#ifndef CARGADOR_H
#define CARGADOR_H

#include "proceso.h"
#include <map>
#include <string>

// Carga procesos desde un archivo en formato especificado
std::map<int, Proceso> cargarProcesos(const std::string &nombreArchivo);

#endif 