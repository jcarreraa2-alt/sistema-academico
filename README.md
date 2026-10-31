NombreRolJoseph Lizardo Carrera AriasDesarrolladorWilliam Alexander Polanco CifuentesDesarrolladorSteven Josué Jaimes PérezDesarrollador
# Sistema Académico

Programa de consola en **C++** para gestionar estudiantes, cursos y notas.  
Proyecto académico desarrollado de forma modular.

---

## Características

- Registro, edición, búsqueda y listado de **estudiantes**
- Gestión de **cursos**
- Registro de **notas**
- Cálculo de promedios usando **funciones recursivas**
- Generación de **reportes**
- Persistencia de datos en archivo

---

## Estructura del proyecto
sistema_academico/
├── Makefile
├── include/                  # Cabeceras (.h)
│   ├── datos.h
│   ├── utilidades.h
│   ├── estudiantes.h
│   ├── cursos_notas.h
│   ├── procesamiento.h
│   └── reportes.h
└── src/                      # Implementaciones (.cpp)
├── main.cpp
├── utilidades.cpp
├── estudiantes.cpp
├── cursos_notas.cpp
├── procesamiento.cpp
└── reportes.cpptext

---

## Requisitos

- Compilador **g++** con soporte para C++11 o superior
- Sistema operativo Linux, macOS o Windows (con MinGW / MSYS2)

---

## Cómo compilar y ejecutar

### Opción 1: Usando Make (recomendado)

```bash
cd sistema_academico
make
./sistema_academico

cd sistema_academico
g++ -std=c++11 -O2 -Wall -o sistema_academico \
    src/main.cpp src/utilidades.cpp src/estudiantes.cpp \
    src/cursos_notas.cpp src/procesamiento.cpp src/reportes.cpp

./sistema_academico
make clean
