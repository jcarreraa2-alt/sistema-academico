/*
 * estudiantes.cpp
 * Implementacion del modulo de gestion de Estudiantes.
 * Origen: sistema_academico.cpp (modulo 1)
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../include/datos.h"
#include "../include/utilidades.h"
#include "../include/estudiantes.h"

using namespace std;

// ------------------------------------------------------------------
// CRUD ESTUDIANTES
// ------------------------------------------------------------------
void registrarEstudiante() {
    cout << "\n--- Registrar Estudiante ---\n";
    string carnet = leerLinea("Carne: ");
    if (buscarEstudiantePorCarnet(carnet) != nullptr) {
        cout << "  >> Error: ya existe un estudiante con ese carne.\n";
        return;
    }
    Estudiante e;
    e.carnet   = carnet;
    e.nombre   = leerLinea("Nombre completo: ");
    e.carrera  = leerLinea("Carrera: ");
    e.semestre = leerEntero("Semestre: ");
    estudiantes.push_back(e);
    cout << "  >> Estudiante registrado correctamente.\n";
}

void editarEstudiante() {
    cout << "\n--- Editar Estudiante ---\n";
    string carnet = leerLinea("Carne del estudiante a editar: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) {
        cout << "  >> Error: no existe un estudiante con ese carne.\n";
        return;
    }

    cout << "Deje en blanco un campo para conservar el valor actual.\n";
    string nombre = leerLinea("Nombre [" + e->nombre + "]: ");
    if (!nombre.empty()) e->nombre = nombre;

    string carrera = leerLinea("Carrera [" + e->carrera + "]: ");
    if (!carrera.empty()) e->carrera = carrera;

    string semestreStr = leerLinea("Semestre [" + to_string(e->semestre) + "]: ");
    if (!semestreStr.empty()) {
        try { e->semestre = stoi(semestreStr); }
        catch (...) { cout << "  >> Valor de semestre invalido, se conserva el anterior.\n"; }
    }
    cout << "  >> Estudiante actualizado correctamente.\n";
}

void buscarEstudiante() {
    cout << "\n--- Buscar Estudiante ---\n1. Por carne\n2. Por nombre\n";
    int opcion = leerEntero("Opcion: ");

    if (opcion == 1) {
        string carnet = leerLinea("Carne: ");
        Estudiante* e = buscarEstudiantePorCarnet(carnet);
        if (e == nullptr) {
            cout << "  >> No se encontro ningun estudiante con ese carne.\n";
            return;
        }
        cout << "\nCarne: "    << e->carnet
             << "\nNombre: "   << e->nombre
             << "\nCarrera: "  << e->carrera
             << "\nSemestre: " << e->semestre << "\n";

    } else if (opcion == 2) {
        string nombre = leerLinea("Nombre (o parte del nombre): ");
        string nombreLower = nombre;
        transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);

        bool encontrado = false;
        for (const auto& e : estudiantes) {
            string nl = e.nombre;
            transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
            if (nl.find(nombreLower) != string::npos) {
                cout << "\nCarne: "    << e.carnet
                     << "\nNombre: "   << e.nombre
                     << "\nCarrera: "  << e.carrera
                     << "\nSemestre: " << e.semestre << "\n";
                encontrado = true;
            }
        }
        if (!encontrado) cout << "  >> No se encontraron coincidencias.\n";
    } else {
        cout << "  >> Opcion invalida.\n";
    }
}

void listarEstudiantes() {
    cout << "\n--- Listado de Estudiantes ---\n";
    if (estudiantes.empty()) {
        cout << "  (No hay estudiantes registrados)\n";
        return;
    }
    cout << left << setw(12) << "Carne"
                 << setw(28) << "Nombre"
                 << setw(20) << "Carrera"
                 << setw(10) << "Semestre" << "\n";
    cout << string(70, '-') << "\n";
    for (const auto& e : estudiantes)
        cout << left << setw(12) << e.carnet
                     << setw(28) << e.nombre
                     << setw(20) << e.carrera
                     << setw(10) << e.semestre << "\n";
}

// ------------------------------------------------------------------
// MENU
// ------------------------------------------------------------------
void menuEstudiantes() {
    int opcion;
    do {
        cout << "\n===== MENU ESTUDIANTES =====\n"
             << "1. Registrar estudiante\n"
             << "2. Editar estudiante\n"
             << "3. Buscar estudiante\n"
             << "4. Listar todos\n"
             << "0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: registrarEstudiante(); break;
            case 2: editarEstudiante();    break;
            case 3: buscarEstudiante();    break;
            case 4: listarEstudiantes();   break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}
