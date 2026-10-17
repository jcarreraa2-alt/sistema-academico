#pragma once
/*
 * reportes.h
 * Declaraciones del modulo de Reportes, Estadisticas y Persistencia.
 */

#include <string>
using namespace std;

void generarReportePromedioEstudiante();
void generarReportePromedioCurso();
void generarReporteAprobadosReprobados();
void generarReporteMejorEstudiante();
void generarExpedientePorCarne();

void guardarDatos(const string& nombreArchivo);
void cargarDatos(const string& nombreArchivo);

void menuReportes();
