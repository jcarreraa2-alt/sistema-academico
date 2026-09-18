/*
 * SISTEMA ACADEMICO - VERSION COMPLETA (UNIFICADA)
 * -----------------------------------------------------------
 * Combina en un solo programa de consola (sin base de datos externa,
 * sin interfaz grafica) los tres modulos que se tenian por separado:
 *
 *   1) sistema_academico.cpp   -> Estudiantes y Cursos/Notas (con validacion)
 *   2) reporte.cpp             -> Reportes y persistencia en archivo de texto
 *   3) Modulo_Procesamiento.cpp -> Calculo de promedios y estadisticas
 *                                  usando funciones recursivas
 *
 * Como los tres archivos originales definian su propio "main()" y sus
 * propias versiones (distintas) de Estudiante/Curso, aqui se unificaron
 * en una sola estructura de datos (usando vector<>), y la logica de cada
 * archivo se adapto para trabajar sobre esa misma estructura.
 *
 * Compilar:
 *    g++ -std=c++11 -O2 -o sistema_academico_completo sistema_academico_completo.cpp
 *
 * Ejecutar:
 *    ./sistema_academico_completo
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>
#include <iomanip>

using namespace std;

// ==================================================================
// CONSTANTES
// ==================================================================
const double NOTA_MINIMA = 0.0;
const double NOTA_MAXIMA = 100.0;
const double NOTA_APROBACION = 61.0;   // umbral usado en los reportes
const string ARCHIVO_DATOS = "datos_academicos.txt";

// ==================================================================
// ESTRUCTURAS DE DATOS (unificadas)
// ==================================================================
struct Estudiante {
    string carnet;
    string nombre;
    string carrera;
    int semestre;
};

struct Curso {
    string codigo;
    string nombre;
    int creditos;
};

struct Nota {
    string carnetEstudiante;
    string codigoCurso;
    double calificacion;
};

// ==================================================================
// ALMACENAMIENTO EN MEMORIA
// ==================================================================
vector<Estudiante> estudiantes;
vector<Curso> cursos;
vector<Nota> notas;

// ==================================================================
// UTILIDADES DE ENTRADA
// ==================================================================
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

// ==================================================================
// BUSQUEDAS BASICAS
// ==================================================================
Estudiante* buscarEstudiantePorCarnet(const string& carnet) {
    for (auto& e : estudiantes) if (e.carnet == carnet) return &e;
    return nullptr;
}

Curso* buscarCursoPorCodigo(const string& codigo) {
    for (auto& c : cursos) if (c.codigo == codigo) return &c;
    return nullptr;
}

// ==================================================================
// MODULO 1: ESTUDIANTES  (de sistema_academico.cpp)
// ==================================================================
void registrarEstudiante() {
    cout << "\n--- Registrar Estudiante ---\n";
    string carnet = leerLinea("Carne: ");
    if (buscarEstudiantePorCarnet(carnet) != nullptr) {
        cout << "  >> Error: ya existe un estudiante con ese carne.\n";
        return;
    }
    Estudiante e;
    e.carnet = carnet;
    e.nombre = leerLinea("Nombre completo: ");
    e.carrera = leerLinea("Carrera: ");
    e.semestre = leerEntero("Semestre: ");
    estudiantes.push_back(e);
    cout << "  >> Estudiante registrado correctamente.\n";
}

void editarEstudiante() {
    cout << "\n--- Editar Estudiante ---\n";
    string carnet = leerLinea("Carne del estudiante a editar: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) { cout << "  >> Error: no existe un estudiante con ese carne.\n"; return; }

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
        if (e == nullptr) { cout << "  >> No se encontro ningun estudiante con ese carne.\n"; return; }
        cout << "\nCarne: " << e->carnet << "\nNombre: " << e->nombre
             << "\nCarrera: " << e->carrera << "\nSemestre: " << e->semestre << "\n";
    } else if (opcion == 2) {
        string nombre = leerLinea("Nombre (o parte del nombre): ");
        string nombreLower = nombre;
        transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);
        bool encontrado = false;
        for (const auto& e : estudiantes) {
            string nl = e.nombre;
            transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
            if (nl.find(nombreLower) != string::npos) {
                cout << "\nCarne: " << e.carnet << "\nNombre: " << e.nombre
                     << "\nCarrera: " << e.carrera << "\nSemestre: " << e.semestre << "\n";
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
    if (estudiantes.empty()) { cout << "  (No hay estudiantes registrados)\n"; return; }
    cout << left << setw(12) << "Carne" << setw(28) << "Nombre"
         << setw(20) << "Carrera" << setw(10) << "Semestre" << "\n";
    cout << string(70, '-') << "\n";
    for (const auto& e : estudiantes)
        cout << left << setw(12) << e.carnet << setw(28) << e.nombre
             << setw(20) << e.carrera << setw(10) << e.semestre << "\n";
}

// ==================================================================
// MODULO 2: CURSOS  (de sistema_academico.cpp)
// ==================================================================
void registrarCurso() {
    cout << "\n--- Registrar Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    if (buscarCursoPorCodigo(codigo) != nullptr) { cout << "  >> Error: ya existe un curso con ese codigo.\n"; return; }
    Curso c;
    c.codigo = codigo;
    c.nombre = leerLinea("Nombre del curso: ");
    c.creditos = leerEntero("Creditos: ");
    cursos.push_back(c);
    cout << "  >> Curso registrado correctamente.\n";
}

void editarCurso() {
    cout << "\n--- Editar Curso ---\n";
    string codigo = leerLinea("Codigo del curso a editar: ");
    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) { cout << "  >> Error: no existe un curso con ese codigo.\n"; return; }
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
    if (c == nullptr) { cout << "  >> No se encontro ningun curso con ese codigo.\n"; return; }
    cout << "\nCodigo: " << c->codigo << "\nNombre: " << c->nombre << "\nCreditos: " << c->creditos << "\n";
}

void listarCursos() {
    cout << "\n--- Listado de Cursos ---\n";
    if (cursos.empty()) { cout << "  (No hay cursos registrados)\n"; return; }
    cout << left << setw(12) << "Codigo" << setw(30) << "Nombre" << setw(10) << "Creditos" << "\n";
    cout << string(52, '-') << "\n";
    for (const auto& c : cursos)
        cout << left << setw(12) << c.codigo << setw(30) << c.nombre << setw(10) << c.creditos << "\n";
}

// ==================================================================
// MODULO 3: NOTAS  (de sistema_academico.cpp)
// ==================================================================
void registrarNota() {
    cout << "\n--- Registrar Nota ---\n";
    string carnet = leerLinea("Carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) { cout << "  >> Error: no existe ningun estudiante con ese carne. No se registro la nota.\n"; return; }

    string codigoCurso = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigoCurso);
    if (c == nullptr) { cout << "  >> Error: no existe ningun curso con ese codigo. No se registro la nota.\n"; return; }

    double valorNota = leerDouble("Nota (" + to_string((int)NOTA_MINIMA) + " a " + to_string((int)NOTA_MAXIMA) + "): ");
    if (valorNota < NOTA_MINIMA || valorNota > NOTA_MAXIMA) {
        cout << "  >> Error: la nota esta fuera del rango permitido (" << NOTA_MINIMA << " - " << NOTA_MAXIMA << "). No se registro.\n";
        return;
    }

    for (auto& n : notas) {
        if (n.carnetEstudiante == carnet && n.codigoCurso == codigoCurso) {
            n.calificacion = valorNota;
            cout << "  >> Ya existia una nota para ese estudiante en ese curso; se actualizo el valor.\n";
            return;
        }
    }
    Nota n; n.carnetEstudiante = carnet; n.codigoCurso = codigoCurso; n.calificacion = valorNota;
    notas.push_back(n);
    cout << "  >> Nota registrada correctamente.\n";
}

void consultarNotasPorEstudiante() {
    cout << "\n--- Consultar Notas por Estudiante ---\n";
    string carnet = leerLinea("Carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) { cout << "  >> Error: no existe ningun estudiante con ese carne.\n"; return; }

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
    if (c == nullptr) { cout << "  >> Error: no existe ningun curso con ese codigo.\n"; return; }

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

// ==================================================================
// MODULO 4: PROCESAMIENTO (RECURSIVO)  (de Modulo_Procesamiento.cpp)
// Adaptado para trabajar sobre el vector<Nota> en lugar de un arreglo fijo.
// ==================================================================

// Suma recursiva de las calificaciones de un estudiante especifico
double sumarCalificacionesRecursivo(const string& carnet, size_t indice) {
    if (indice >= notas.size()) return 0.0;               // caso base
    double resto = sumarCalificacionesRecursivo(carnet, indice + 1);
    if (notas[indice].carnetEstudiante == carnet) return notas[indice].calificacion + resto;
    return resto;
}

// Cuenta recursivamente cuantas notas tiene un estudiante especifico
int contarNotasRecursivo(const string& carnet, size_t indice) {
    if (indice >= notas.size()) return 0;                 // caso base
    int resto = contarNotasRecursivo(carnet, indice + 1);
    if (notas[indice].carnetEstudiante == carnet) return 1 + resto;
    return resto;
}

// Cuenta recursivamente cuantas notas (de todo el sistema) son aprobatorias
int contarAprobadosRecursivo(size_t indice) {
    if (indice >= notas.size()) return 0;                 // caso base
    int actual = (notas[indice].calificacion >= NOTA_APROBACION) ? 1 : 0;
    return actual + contarAprobadosRecursivo(indice + 1);
}

double calcularPromedioEstudiante(const string& carnet) {
    int cantidad = contarNotasRecursivo(carnet, 0);
    if (cantidad == 0) return 0.0;
    double suma = sumarCalificacionesRecursivo(carnet, 0);
    return suma / cantidad;
}

string determinarEstado(double promedio) {
    return (promedio >= NOTA_APROBACION) ? "APROBADO" : "REPROBADO";
}

// ==================================================================
// MODULO 5: REPORTES  (de reporte.cpp, adaptado a la nueva estructura)
// ==================================================================
void generarReportePromedioEstudiante() {
    cout << "\n--- REPORTE: PROMEDIO POR ESTUDIANTE ---\n";
    if (estudiantes.empty()) { cout << "No hay estudiantes registrados.\n"; return; }
    for (const auto& e : estudiantes) {
        double prom = calcularPromedioEstudiante(e.carnet);
        int cant = contarNotasRecursivo(e.carnet, 0);
        cout << "Carne: " << e.carnet << " | " << e.nombre
             << " | Notas: " << cant
             << " | Promedio: " << fixed << setprecision(2) << prom
             << " | Estado: " << (cant > 0 ? determinarEstado(prom) : "SIN NOTAS") << "\n";
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
        cout << "Curso: " << c->nombre << " (" << cursoBuscado << ") | Promedio General: "
             << fixed << setprecision(2) << (sumaNotas / contador)
             << " (Total de alumnos evaluados: " << contador << ")\n";
    } else {
        cout << "No se encontraron notas registradas para ese curso.\n";
    }
}

void generarReporteAprobadosReprobados() {
    cout << "\n--- REPORTE: APROBADOS Y REPROBADOS ---\n";
    int aprobados = contarAprobadosRecursivo(0);
    int reprobados = (int)notas.size() - aprobados;
    cout << "Total de notas aprobadas   : " << aprobados << "\n";
    cout << "Total de notas reprobadas  : " << reprobados << "\n";
}

void generarReporteMejorEstudiante() {
    cout << "\n--- REPORTE: ESTUDIANTE CON MAYOR PROMEDIO ---\n";
    if (estudiantes.empty()) { cout << "No hay estudiantes registrados.\n"; return; }

    string mejorNombre; double mayorPromedio = -1.0; bool alguno = false;
    for (const auto& e : estudiantes) {
        if (contarNotasRecursivo(e.carnet, 0) == 0) continue;
        double prom = calcularPromedioEstudiante(e.carnet);
        if (prom > mayorPromedio) {
            mayorPromedio = prom;
            mejorNombre = e.nombre + " (" + e.carnet + ")";
            alguno = true;
        }
    }
    if (alguno) cout << "El estudiante con mayor promedio es: " << mejorNombre
                      << " con " << fixed << setprecision(2) << mayorPromedio << "\n";
    else cout << "No hay suficientes datos de notas.\n";
}

void generarExpedientePorCarne() {
    cout << "\n--- REPORTE: EXPEDIENTE POR CARNE ---\n";
    string carneBuscado = leerLinea("Ingrese el numero de carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carneBuscado);
    if (e == nullptr) { cout << "Estudiante con carne " << carneBuscado << " no encontrado.\n"; return; }

    cout << "\n========================================\n";
    cout << "EXPEDIENTE ACADEMICO\n";
    cout << "Carne: " << e->carnet << "\nNombre: " << e->nombre
         << "\nCarrera: " << e->carrera << "\nSemestre: " << e->semestre << "\n";
    cout << "Historial de Cursos:\n";
    bool alguno = false;
    for (const auto& n : notas) {
        if (n.carnetEstudiante == carneBuscado) {
            Curso* c = buscarCursoPorCodigo(n.codigoCurso);
            string nombreCurso = (c != nullptr) ? c->nombre : "(curso no encontrado)";
            cout << " - [" << n.codigoCurso << "] " << nombreCurso << " | Nota: "
                 << fixed << setprecision(2) << n.calificacion << " | "
                 << (n.calificacion >= NOTA_APROBACION ? "Aprobado" : "Reprobado") << "\n";
            alguno = true;
        }
    }
    if (!alguno) cout << " (Sin notas registradas)\n";
    cout << "========================================\n";
}

// ==================================================================
// PERSISTENCIA EN ARCHIVO DE TEXTO  (de reporte.cpp, adaptado)
// ==================================================================
void guardarDatos(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) { cout << "  >> Error al abrir el archivo para guardar.\n"; return; }

    archivo << estudiantes.size() << "\n";
    for (const auto& e : estudiantes)
        archivo << e.carnet << "\n" << e.nombre << "\n" << e.carrera << "\n" << e.semestre << "\n";

    archivo << cursos.size() << "\n";
    for (const auto& c : cursos)
        archivo << c.codigo << "\n" << c.nombre << "\n" << c.creditos << "\n";

    archivo << notas.size() << "\n";
    for (const auto& n : notas)
        archivo << n.carnetEstudiante << "\n" << n.codigoCurso << "\n" << n.calificacion << "\n";

    archivo.close();
    cout << "  >> Datos guardados en \"" << nombreArchivo << "\".\n";
}

void cargarDatos(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "  >> No se encontro \"" << nombreArchivo << "\"; se inicia con datos vacios.\n";
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
         << estudiantes.size() << " estudiantes, " << cursos.size() << " cursos, "
         << notas.size() << " notas).\n";
}

// ==================================================================
// MENUS
// ==================================================================
void menuEstudiantes() {
    int opcion;
    do {
        cout << "\n===== MENU ESTUDIANTES =====\n"
             << "1. Registrar estudiante\n2. Editar estudiante\n"
             << "3. Buscar estudiante\n4. Listar todos\n0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: registrarEstudiante(); break;
            case 2: editarEstudiante(); break;
            case 3: buscarEstudiante(); break;
            case 4: listarEstudiantes(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void menuCursosNotas() {
    int opcion;
    do {
        cout << "\n===== MENU CURSOS / NOTAS =====\n"
             << "1. Registrar curso\n2. Editar curso\n3. Buscar curso\n4. Listar cursos\n"
             << "5. Registrar nota\n6. Notas por estudiante\n7. Notas por curso\n0. Volver\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: registrarCurso(); break;
            case 2: editarCurso(); break;
            case 3: buscarCurso(); break;
            case 4: listarCursos(); break;
            case 5: registrarNota(); break;
            case 6: consultarNotasPorEstudiante(); break;
            case 7: consultarNotasPorCurso(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

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
            case 1: generarReportePromedioEstudiante(); break;
            case 2: generarReportePromedioCurso(); break;
            case 3: generarReporteAprobadosReprobados(); break;
            case 4: generarReporteMejorEstudiante(); break;
            case 5: generarExpedientePorCarne(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

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
            case 1: menuEstudiantes(); break;
            case 2: menuCursosNotas(); break;
            case 3: menuReportes(); break;
            case 4: guardarDatos(ARCHIVO_DATOS); break;
            case 5: cargarDatos(ARCHIVO_DATOS); break;
            case 0: cout << "Saliendo del sistema...\n"; break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
    return 0;
}
