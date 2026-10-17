#pragma once
/*
 * utilidades.h
 * Funciones de entrada de datos y busquedas basicas.
 */

#include <string>
#include "datos.h"

void       limpiarBuffer();
string     leerLinea(const string& mensaje);
int        leerEntero(const string& mensaje);
double     leerDouble(const string& mensaje);

Estudiante* buscarEstudiantePorCarnet(const string& carnet);
Curso*      buscarCursoPorCodigo(const string& codigo);
