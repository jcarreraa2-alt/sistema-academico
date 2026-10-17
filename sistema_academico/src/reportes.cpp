/*
 * reportes.cpp
 * Implementacion del modulo de Reportes, Estadisticas y Persistencia.
 * Origen: reporte.cpp (adaptado a la estructura unificada)
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include "../include/datos.h"
#include "../include/utilidades.h"
#include "../include/procesamiento.h"
#include "../include/reportes.h"

using namespace std;

// ------------------------------------------------------------------
// REPORTES Y ESTADISTICAS
// ------------------------------------------------------------------
void generarReportePromedioEstudiante() {
    cout << "\n--- REPORTE: PROMEDIO POR ESTUDIANTE ---\n";
    if (estudiantes.empty()) { cout << "No hay estudiantes registrados.\n"; return; }

    for (const auto& e : estudiantes) {
        double prom = calcularPromedioEstudiante(e.carnet);
        int    cant = contarNotasRecursivo(e.carnet, 0);
        cout << "Carne: "    << e.carnet
             << " | "        << e.nombre
             << " | Notas: " << cant
             << " | Promedio: " << fixed << setprecision(2) << prom
             << " | Estado: "  << (cant > 0 ? determinarEstado(prom) : "SIN NOTAS") << "\n";
    }
}

void generarReportePromedioCurso() {
    cout << "\n--- REPORTE: PROMEDIO POR CURSO ---\n";
    string cursoBuscado = leerLinea("Ingrese el codigo del curso a consultar: ");
    Curso* c = buscarCursoPorCodigo(cursoBuscado);
    if (c == nullptr) { cout << "  >> Error: no existe un curso con ese codigo.\n"; return; }

    double sumaNotas = 0; int contador = 0;
    for (const auto& n : notas) {
        if (n.codigoCurso == cursoBuscado) { sumaNotas += n.calificacion; contador++; }
    }

    if (contador > 0) {
        cout << "Curso: " << c->nombre << " (" << cursoBuscado << ")"
             << " | Promedio General: " << fixed << setprecision(2) << (sumaNotas / contador)
             << " (Total de alumnos evaluados: " << contador << ")\n";
    } else {
        cout << "No se encontraron notas registradas para ese curso.\n";
    }
}

void generarReporteAprobadosReprobados() {
    cout << "\n--- REPORTE: APROBADOS Y REPROBADOS ---\n";
    int aprobados  = contarAprobadosRecursivo(0);
    int reprobados = (int)notas.size() - aprobados;
    cout << "Total de notas aprobadas  : " << aprobados  << "\n";
    cout << "Total de notas reprobadas : " << reprobados << "\n";
}

void generarReporteMejorEstudiante() {
    cout << "\n--- REPORTE: ESTUDIANTE CON MAYOR PROMEDIO ---\n";
    if (estudiantes.empty()) { cout << "No hay estudiantes registrados.\n"; return; }

    string mejorNombre;
    double mayorPromedio = -1.0;
    bool   alguno        = false;

    for (const auto& e : estudiantes) {
        if (contarNotasRecursivo(e.carnet, 0) == 0) continue;
        double prom = calcularPromedioEstudiante(e.carnet);
        if (prom > mayorPromedio) {
            mayorPromedio = prom;
            mejorNombre   = e.nombre + " (" + e.carnet + ")";
            alguno        = true;
        }
    }

    if (alguno)
        cout << "El estudiante con mayor promedio es: " << mejorNombre
             << " con " << fixed << setprecision(2) << mayorPromedio << "\n";
    else
        cout << "No hay suficientes datos de notas.\n";
}

void generarExpedientePorCarne() {
    cout << "\n--- REPORTE: EXPEDIENTE POR CARNE ---\n";
    string carneBuscado = leerLinea("Ingrese el numero de carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carneBuscado);
    if (e == nullptr) {
        cout << "Estudiante con carne " << carneBuscado << " no encontrado.\n";
        return;
    }

    cout << "\n========================================\n"
         << "EXPEDIENTE ACADEMICO\n"
         << "Carne: "    << e->carnet
         << "\nNombre: "  << e->nombre
         << "\nCarrera: " << e->carrera
         << "\nSemestre: "<< e->semestre
         << "\nHistorial de Cursos:\n";

    bool alguno = false;
    for (const auto& n : notas) {
        if (n.carnetEstudiante == carneBuscado) {
            Curso* c = buscarCursoPorCodigo(n.codigoCurso);
            string nombreCurso = (c != nullptr) ? c->nombre : "(curso no encontrado)";
            cout << " - [" << n.codigoCurso << "] " << nombreCurso
                 << " | Nota: " << fixed << setprecision(2) << n.calificacion
                 << " | " << (n.calificacion >= NOTA_APROBACION ? "Aprobado" : "Reprobado") << "\n";
            alguno = true;
        }
    }
    if (!alguno) cout << " (Sin notas registradas)\n";
    cout << "========================================\n";
}

// ------------------------------------------------------------------
// PERSISTENCIA EN ARCHIVO
// ------------------------------------------------------------------
void guardarDatos(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "  >> Error al abrir el archivo para guardar.\n";
        return;
    }

    archivo << estudiantes.size() << "\n";
    for (const auto& e : estudiantes)
        archivo << e.carnet << "\n" << e.nombre << "\n"
                << e.carrera << "\n" << e.semestre << "\n";

    archivo << cursos.size() << "\n";
    for (const auto& c : cursos)
        archivo << c.codigo << "\n" << c.nombre << "\n" << c.creditos << "\n";

    archivo << notas.size() << "\n";
    for (const auto& n : notas)
        archivo << n.carnetEstudiante << "\n"
                << n.codigoCurso      << "\n"
                << n.calificacion     << "\n";

    archivo.close();
    cout << "  >> Datos guardados en \"" << nombreArchivo << "\".\n";
}

void cargarDatos(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "  >> No se encontro \"" << nombreArchivo
             << "\"; se inicia con datos vacios.\n";
        return;
    }

    estudiantes.clear(); cursos.clear(); notas.clear();

    size_t totalEst; archivo >> totalEst; archivo.ignore();
    for (size_t i = 0; i < totalEst; i++) {
        Estudiante e;
        getline(archivo, e.carnet);
        getline(archivo, e.nombre);
        getline(archivo, e.carrera);
        archivo >> e.semestre; archivo.ignore();
        estudiantes.push_back(e);
    }

    size_t totalCur; archivo >> totalCur; archivo.ignore();
    for (size_t i = 0; i < totalCur; i++) {
        Curso c;
        getline(archivo, c.codigo);
        getline(archivo, c.nombre);
        archivo >> c.creditos; archivo.ignore();
        cursos.push_back(c);
    }

    size_t totalNot; archivo >> totalNot; archivo.ignore();
    for (size_t i = 0; i < totalNot; i++) {
        Nota n;
        getline(archivo, n.carnetEstudiante);
        getline(archivo, n.codigoCurso);
        archivo >> n.calificacion; archivo.ignore();
        notas.push_back(n);
    }

    archivo.close();
    cout << "  >> Datos cargados desde \"" << nombreArchivo << "\" ("
         << estudiantes.size() << " estudiantes, "
         << cursos.size()      << " cursos, "
         << notas.size()       << " notas).\n";
}

// ------------------------------------------------------------------
// MENU
// ------------------------------------------------------------------
void menuReportes() {
    int opcion;
    do {
        cout << "\n===== MENU REPORTES Y ESTADISTICAS =====\n"
             << "1. Promedio por estudiante (todos)\n"
             << "2. Promedio por curso\n"
             << "3. Aprobados y reprobados (total de notas)\n"
             << "4. Estudiante con mayor promedio\n"
             << "5. Expediente por carne\n"
             << "0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: generarReportePromedioEstudiante();    break;
            case 2: generarReportePromedioCurso();         break;
            case 3: generarReporteAprobadosReprobados();   break;
            case 4: generarReporteMejorEstudiante();       break;
            case 5: generarExpedientePorCarne();           break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}
