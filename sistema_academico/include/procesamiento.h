#pragma once
/*
 * procesamiento.h
 * Declaraciones del modulo de calculo de promedios y estadisticas
 * usando funciones recursivas.
 */

#include <string>
using namespace std;

double sumarCalificacionesRecursivo(const string& carnet, size_t indice);
int    contarNotasRecursivo(const string& carnet, size_t indice);
int    contarAprobadosRecursivo(size_t indice);
double calcularPromedioEstudiante(const string& carnet);
string determinarEstado(double promedio);
