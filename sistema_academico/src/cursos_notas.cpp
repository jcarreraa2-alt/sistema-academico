/*
 * cursos_notas.cpp
 * Implementacion del modulo de gestion de Cursos y Notas.
 * Origen: sistema_academico.cpp (modulos 2 y 3)
 */

#include <iostream>
#include <iomanip>
#include "../include/datos.h"
#include "../include/utilidades.h"
#include "../include/cursos_notas.h"

using namespace std;

// ------------------------------------------------------------------
// CRUD CURSOS
// ------------------------------------------------------------------
void registrarCurso() {
    cout << "\n--- Registrar Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    if (buscarCursoPorCodigo(codigo) != nullptr) {
        cout << "  >> Error: ya existe un curso con ese codigo.\n";
        return;
    }
    Curso c;
    c.codigo   = codigo;
    c.nombre   = leerLinea("Nombre del curso: ");
    c.creditos = leerEntero("Creditos: ");
    cursos.push_back(c);
    cout << "  >> Curso registrado correctamente.\n";
}

void editarCurso() {
    cout << "\n--- Editar Curso ---\n";
    string codigo = leerLinea("Codigo del curso a editar: ");
    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) {
        cout << "  >> Error: no existe un curso con ese codigo.\n";
        return;
    }
    cout << "Deje en blanco un campo para conservar el valor actual.\n";
    string nombre = leerLinea("Nombre [" + c->nombre + "]: ");
    if (!nombre.empty()) c->nombre = nombre;

    string creditosStr = leerLinea("Creditos [" + to_string(c->creditos) + "]: ");
    if (!creditosStr.empty()) {
        try { c->creditos = stoi(creditosStr); }
        catch (...) { cout << "  >> Valor de creditos invalido, se conserva el anterior.\n"; }
    }
    cout << "  >> Curso actualizado correctamente.\n";
}

void buscarCurso() {
    cout << "\n--- Buscar Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) {
        cout << "  >> No se encontro ningun curso con ese codigo.\n";
        return;
    }
    cout << "\nCodigo: "   << c->codigo
         << "\nNombre: "   << c->nombre
         << "\nCreditos: " << c->creditos << "\n";
}

void listarCursos() {
    cout << "\n--- Listado de Cursos ---\n";
    if (cursos.empty()) {
        cout << "  (No hay cursos registrados)\n";
        return;
    }
    cout << left << setw(12) << "Codigo"
                 << setw(30) << "Nombre"
                 << setw(10) << "Creditos" << "\n";
    cout << string(52, '-') << "\n";
    for (const auto& c : cursos)
        cout << left << setw(12) << c.codigo
                     << setw(30) << c.nombre
                     << setw(10) << c.creditos << "\n";
}

// ------------------------------------------------------------------
// CRUD NOTAS
// ------------------------------------------------------------------
void registrarNota() {
    cout << "\n--- Registrar Nota ---\n";
    string carnet = leerLinea("Carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) {
        cout << "  >> Error: no existe ningun estudiante con ese carne. No se registro la nota.\n";
        return;
    }

    string codigoCurso = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigoCurso);
    if (c == nullptr) {
        cout << "  >> Error: no existe ningun curso con ese codigo. No se registro la nota.\n";
        return;
    }

    double valorNota = leerDouble("Nota (" + to_string((int)NOTA_MINIMA) + " a "
                                            + to_string((int)NOTA_MAXIMA) + "): ");
    if (valorNota < NOTA_MINIMA || valorNota > NOTA_MAXIMA) {
        cout << "  >> Error: la nota esta fuera del rango permitido ("
             << NOTA_MINIMA << " - " << NOTA_MAXIMA << "). No se registro.\n";
        return;
    }

    // Actualizar si ya existe
    for (auto& n : notas) {
        if (n.carnetEstudiante == carnet && n.codigoCurso == codigoCurso) {
            n.calificacion = valorNota;
            cout << "  >> Ya existia una nota para ese estudiante en ese curso; se actualizo el valor.\n";
            return;
        }
    }

    Nota n;
    n.carnetEstudiante = carnet;
    n.codigoCurso      = codigoCurso;
    n.calificacion     = valorNota;
    notas.push_back(n);
    cout << "  >> Nota registrada correctamente.\n";
}

void consultarNotasPorEstudiante() {
    cout << "\n--- Consultar Notas por Estudiante ---\n";
    string carnet = leerLinea("Carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) {
        cout << "  >> Error: no existe ningun estudiante con ese carne.\n";
        return;
    }

    cout << "\nNotas de " << e->nombre << " (" << e->carnet << "):\n";
    bool encontrado = false;
    for (const auto& n : notas) {
        if (n.carnetEstudiante == carnet) {
            Curso* c = buscarCursoPorCodigo(n.codigoCurso);
            string nombreCurso = (c != nullptr) ? c->nombre : "(curso no encontrado)";
            cout << "  - " << n.codigoCurso << " " << nombreCurso << ": "
                 << fixed << setprecision(2) << n.calificacion << "\n";
            encontrado = true;
        }
    }
    if (!encontrado) cout << "  (Este estudiante no tiene notas registradas)\n";
}

void consultarNotasPorCurso() {
    cout << "\n--- Consultar Notas por Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) {
        cout << "  >> Error: no existe ningun curso con ese codigo.\n";
        return;
    }

    cout << "\nNotas del curso " << c->nombre << " (" << c->codigo << "):\n";
    bool encontrado = false;
    for (const auto& n : notas) {
        if (n.codigoCurso == codigo) {
            Estudiante* e = buscarEstudiantePorCarnet(n.carnetEstudiante);
            string nombreEst = (e != nullptr) ? e->nombre : "(estudiante no encontrado)";
            cout << "  - " << n.carnetEstudiante << " " << nombreEst << ": "
                 << fixed << setprecision(2) << n.calificacion << "\n";
            encontrado = true;
        }
    }
    if (!encontrado) cout << "  (Este curso no tiene notas registradas)\n";
}

// ------------------------------------------------------------------
// MENU
// ------------------------------------------------------------------
void menuCursosNotas() {
    int opcion;
    do {
        cout << "\n===== MENU CURSOS / NOTAS =====\n"
             << "1. Registrar curso\n"
             << "2. Editar curso\n"
             << "3. Buscar curso\n"
             << "4. Listar cursos\n"
             << "5. Registrar nota\n"
             << "6. Notas por estudiante\n"
             << "7. Notas por curso\n"
             << "0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: registrarCurso();             break;
            case 2: editarCurso();                break;
            case 3: buscarCurso();                break;
            case 4: listarCursos();               break;
            case 5: registrarNota();              break;
            case 6: consultarNotasPorEstudiante(); break;
            case 7: consultarNotasPorCurso();     break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}
