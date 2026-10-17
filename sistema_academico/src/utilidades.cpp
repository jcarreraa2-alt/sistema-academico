/*
 * utilidades.cpp
 * Implementacion de funciones de entrada de datos y busquedas basicas.
 */

#include <iostream>
#include <limits>
#include "../include/utilidades.h"

using namespace std;

// ------------------------------------------------------------------
// ENTRADA DE DATOS
// ------------------------------------------------------------------
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string leerLinea(const string& mensaje) {
    string valor;
    cout << mensaje;
    getline(cin, valor);
    return valor;
}

int leerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) { limpiarBuffer(); return valor; }
        cout << "  >> Entrada invalida. Ingrese un numero entero.\n";
        limpiarBuffer();
    }
}

double leerDouble(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) { limpiarBuffer(); return valor; }
        cout << "  >> Entrada invalida. Ingrese un numero.\n";
        limpiarBuffer();
    }
}

// ------------------------------------------------------------------
// BUSQUEDAS BASICAS
// ------------------------------------------------------------------
Estudiante* buscarEstudiantePorCarnet(const string& carnet) {
    for (auto& e : estudiantes)
        if (e.carnet == carnet) return &e;
    return nullptr;
}

Curso* buscarCursoPorCodigo(const string& codigo) {
    for (auto& c : cursos)
        if (c.codigo == codigo) return &c;
    return nullptr;
}
