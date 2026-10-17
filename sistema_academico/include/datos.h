#pragma once
/*
 * datos.h
 * Estructuras de datos, constantes y almacenamiento global compartido
 * por todos los modulos del Sistema Academico.
 */

#include <string>
#include <vector>

using namespace std;

// ------------------------------------------------------------------
// CONSTANTES
// ------------------------------------------------------------------
const double NOTA_MINIMA    = 0.0;
const double NOTA_MAXIMA    = 100.0;
const double NOTA_APROBACION = 61.0;
const string ARCHIVO_DATOS  = "datos_academicos.txt";

// ------------------------------------------------------------------
// ESTRUCTURAS
// ------------------------------------------------------------------
struct Estudiante {
    string carnet;
    string nombre;
    string carrera;
    int    semestre;
};

struct Curso {
    string codigo;
    string nombre;
    int    creditos;
};

struct Nota {
    string carnetEstudiante;
    string codigoCurso;
    double calificacion;
};

// ------------------------------------------------------------------
// ALMACENAMIENTO EN MEMORIA (definido en main.cpp)
// ------------------------------------------------------------------
extern vector<Estudiante> estudiantes;
extern vector<Curso>      cursos;
extern vector<Nota>       notas;
