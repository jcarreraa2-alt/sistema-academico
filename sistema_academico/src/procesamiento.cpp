/*
 * procesamiento.cpp
 * Implementacion del modulo de calculo de promedios y estadisticas
 * usando funciones recursivas.
 * Origen: Modulo_Procesamiento.cpp
 */

#include "../include/datos.h"
#include "../include/procesamiento.h"

// ------------------------------------------------------------------
// FUNCIONES RECURSIVAS
// ------------------------------------------------------------------

// Suma recursiva de las calificaciones de un estudiante especifico
double sumarCalificacionesRecursivo(const string& carnet, size_t indice) {
    if (indice >= notas.size()) return 0.0;   // caso base
    double resto = sumarCalificacionesRecursivo(carnet, indice + 1);
    if (notas[indice].carnetEstudiante == carnet)
        return notas[indice].calificacion + resto;
    return resto;
}

// Cuenta recursivamente cuantas notas tiene un estudiante especifico
int contarNotasRecursivo(const string& carnet, size_t indice) {
    if (indice >= notas.size()) return 0;     // caso base
    int resto = contarNotasRecursivo(carnet, indice + 1);
    if (notas[indice].carnetEstudiante == carnet) return 1 + resto;
    return resto;
}

// Cuenta recursivamente cuantas notas del sistema son aprobatorias
int contarAprobadosRecursivo(size_t indice) {
    if (indice >= notas.size()) return 0;     // caso base
    int actual = (notas[indice].calificacion >= NOTA_APROBACION) ? 1 : 0;
    return actual + contarAprobadosRecursivo(indice + 1);
}

// ------------------------------------------------------------------
// CALCULO DE PROMEDIO Y ESTADO
// ------------------------------------------------------------------
double calcularPromedioEstudiante(const string& carnet) {
    int cantidad = contarNotasRecursivo(carnet, 0);
    if (cantidad == 0) return 0.0;
    return sumarCalificacionesRecursivo(carnet, 0) / cantidad;
}

string determinarEstado(double promedio) {
    return (promedio >= NOTA_APROBACION) ? "APROBADO" : "REPROBADO";
}
