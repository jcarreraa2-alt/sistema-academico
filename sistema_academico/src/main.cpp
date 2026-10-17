/*
 * main.cpp
 * Punto de entrada del Sistema Academico.
 * Define las variables globales compartidas y el menu principal.
 *
 * Compilar:
 *   g++ -std=c++11 -O2 -o sistema_academico \
 *       src/main.cpp src/utilidades.cpp src/estudiantes.cpp \
 *       src/cursos_notas.cpp src/procesamiento.cpp src/reportes.cpp
 *
 * Ejecutar:
 *   ./sistema_academico
 */

#include <iostream>
#include "../include/datos.h"
#include "../include/utilidades.h"
#include "../include/estudiantes.h"
#include "../include/cursos_notas.h"
#include "../include/reportes.h"

using namespace std;

// ------------------------------------------------------------------
// DEFINICION DE VARIABLES GLOBALES (declaradas extern en datos.h)
// ------------------------------------------------------------------
vector<Estudiante> estudiantes;
vector<Curso>      cursos;
vector<Nota>       notas;

// ------------------------------------------------------------------
// MENU PRINCIPAL
// ------------------------------------------------------------------
int main() {
    int opcion;
    do {
        cout << "\n=========================================\n"
             << "   SISTEMA ACADEMICO - MENU PRINCIPAL\n"
             << "=========================================\n"
             << "1. Modulo Estudiantes\n"
             << "2. Modulo Cursos / Notas\n"
             << "3. Modulo Reportes y Estadisticas\n"
             << "4. Guardar datos en archivo\n"
             << "5. Cargar datos desde archivo\n"
             << "0. Salir\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: menuEstudiantes();           break;
            case 2: menuCursosNotas();           break;
            case 3: menuReportes();              break;
            case 4: guardarDatos(ARCHIVO_DATOS); break;
            case 5: cargarDatos(ARCHIVO_DATOS);  break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
    return 0;
}
