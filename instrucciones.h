#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <vector>
#include <string>

using namespace std;

//Se verifica si la instrucción es valida
bool esInstruccionValida(const string& instruccion);
vector<string> cargarInstrucciones(int pid);

#endif
