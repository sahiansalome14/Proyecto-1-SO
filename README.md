# Proyecto-1-SO
 
# Simulador Round Robin - Planificador de Procesos

Un simulador del algoritmo de planificación **Round Robin** implementado en C++. Permite visualizar paso a paso la ejecución de múltiples procesos con quantum de tiempo asignado.

## Miembros del equipo
* Kadiha Nahir Muhamad Orta
* Mariamny Ramirez
* Sahian Salome Gutierrez Ossa
* **Sistema Operativo:** WSL Linux
* **Lenguaje de programación:** C++17
* **Herramientas:** Visual Studio Code

## Descripción del Proyecto

Este proyecto implementa un **simulador completo del algoritmo Round Robin** que incluye:

1. **Planificador Round Robin**
   * Gestiona la ejecución de múltiples procesos con quantum de tiempo
   * Maneja cambios de contexto entre procesos
   * Simula bloqueos aleatorios durante la ejecución

2. **Sistema de Instrucciones**
   * Soporta operaciones aritméticas (ADD, SUB, MUL, INC)
   * Maneja saltos condicionales (JMP) con límites de repetición
   * Valida sintaxis usando expresiones regulares

## Componentes del Programa

```
├── main.cpp                 # Punto de entrada y interfaz principal
├── cargador.cpp            # Carga y validación de procesos desde archivo
├── cargador.h              # Header del cargador de procesos
├── roundRobin.cpp          # Implementación del algoritmo Round Robin
├── roundRobin.h            # Definiciones del planificador
├── instrucciones.cpp       # Procesamiento y validación de instrucciones
├── instrucciones.h         # Header del manejo de instrucciones
├── proceso.h               # Estructura de datos del proceso
└── procesos.txt            # Archivo de configuración de procesos
```

## Instalación

```bash
# Compilar todos los archivos
g++ main.cpp cargador.cpp instrucciones.cpp roundRobin.cpp -o ejecutable
```

## Uso

```bash
./ejecutable
```

El programa solicitará el nombre del archivo de procesos:
```
Ingrese nombre del archivo de procesos: procesos.txt
```

## Formato de Archivos

### procesos.txt
```
PID:1, AX=0, BX=1, CX=2, Quantum=3
PID:2, AX=1, BX=3, CX=4, Quantum=2
PID:3, AX=2, BX=5, CX=6, Quantum=1
```

### Archivos de Instrucciones ([PID].txt)
```
ADD AX, 10
SUB BX, 5
MUL CX, 2
INC AX
JMP 0
NOP
```

## Instrucciones Soportadas

| Instrucción | Sintaxis | Ejemplo |
|-------------|----------|---------|
| **ADD** | `ADD registro, valor/registro` | `ADD AX, 10` |
| **SUB** | `SUB registro, valor/registro` | `SUB BX, CX` |
| **MUL** | `MUL registro, valor/registro` | `MUL CX, 3` |
| **INC** | `INC registro` | `INC AX` |
| **JMP** | `JMP línea` | `JMP 0` |
| **NOP** | `NOP` | `NOP` |

## Funcionalidades

- **Simulación paso a paso**: Control interactivo para análisis detallado
- **Bloqueos aleatorios**: Los procesos pueden bloquearse durante la ejecución (30% probabilidad)
- **Validación robusta**: Verificación de sintaxis usando expresiones regulares
- **Logging automático**: Archivo `Seguimiento.log` con trazabilidad completa
- **Interfaz visual**: Tablas con estado actual de todos los procesos
- **Límites de salto**: Máximo 2 saltos a la misma línea (previene bucles infinitos)

