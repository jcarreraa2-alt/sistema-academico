integrantes 
JOSEPH LIZARDO CARRERA ARIAS
WILLIAM ALEXANDER POLANCO CIFUENTES
STEVEN JOSUÉ JAIMES PÉREZ
Sistema Académico (C++)

Programa de consola en C++ para gestionar estudiantes, cursos y notas. Toda la información se mantiene en memoria mientras el programa está en ejecución (no usa base de datos ni archivos), usando estructuras y vectores de la STL.

Funcionalidades
Módulo Estudiantes
Registrar estudiante (carné, nombre completo, carrera, semestre)
Editar estudiante (puede dejar campos en blanco para conservar el valor actual)
Buscar estudiante (por carné exacto o por coincidencia de nombre)
Consultar / listar todos los estudiantes
Módulo Cursos / Notas
Registrar curso (código, nombre, créditos)
Editar curso (puede dejar campos en blanco para conservar el valor actual)
Buscar curso por código
Listar todos los cursos
Registrar nota (si ya existe una nota para ese estudiante en ese curso, se actualiza en lugar de duplicarse)
Consultar notas por estudiante
Consultar notas por curso
Reglas de negocio aplicadas
No se puede registrar un estudiante con un carné que ya existe.
No se puede registrar un curso con un código que ya existe.
No se puede registrar una nota para un estudiante o curso inexistente.
Las notas deben estar dentro del rango permitido (0 a 100).
Si ya existe una nota de un estudiante en un curso, se actualiza en vez de crear un registro duplicado.
Requisitos
Compilador con soporte para C++17 (g++, MinGW, Dev-C++, etc.).
Cómo compilar
bash
g++ -std=c++17 -O2 -o sistema_academico sistema_academico.cpp
Cómo ejecutar

En Linux/Mac:

bash
./sistema_academico

En Windows (cmd o PowerShell):

sistema_academico.exe
Estructura del menú
MENU PRINCIPAL
├── 1. Modulo Estudiantes
│     ├── 1. Registrar estudiante
│     ├── 2. Editar estudiante
│     ├── 3. Buscar estudiante (por carné o por nombre)
│     ├── 4. Consultar / listar todos
│     └── 0. Volver al menú principal
├── 2. Modulo Cursos / Notas
│     ├── 1. Registrar curso
│     ├── 2. Editar curso
│     ├── 3. Buscar curso
│     ├── 4. Listar cursos
│     ├── 5. Registrar nota
│     ├── 6. Consultar notas por estudiante
│     ├── 7. Consultar notas por curso
│     └── 0. Volver al menú principal
└── 0. Salir
