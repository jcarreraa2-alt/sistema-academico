/*
 * SISTEMA ACADEMICO - VERSION CORREGIDA
 * -----------------------------------------------------------
 * Programa de consola que integra:
 *   1) Estudiantes y cursos (con validaciones)
 *   2) Registro de notas
 *   3) Reportes y promedios (funciones recursivas)
 *   4) Persistencia en archivo de texto (carga automatica al iniciar
 *      y guardado automatico al salir)
 *
 * Compilar:
 *    g++ -std=c++11 -Wall -Wextra -O2 -o sistema_academico_corregido sistema_academico_corregido.cpp
 *
 * Ejecutar:
 *    ./sistema_academico_corregido
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <stdexcept>

using namespace std;

// ==================================================================
// CONSTANTES
// ==================================================================
const double NOTA_MINIMA = 0.0;
const double NOTA_MAXIMA = 100.0;
const double NOTA_APROBACION = 61.0;
const int SEMESTRE_MINIMO = 1;
const int SEMESTRE_MAXIMO = 20;
const int CREDITOS_MINIMOS = 1;
const int CREDITOS_MAXIMOS = 30;
const string ARCHIVO_DATOS = "datos_academicos.txt";

// ==================================================================
// ESTRUCTURAS DE DATOS
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
// UTILIDADES DE TEXTO Y ENTRADA
// ==================================================================

// Quita espacios y saltos de linea al inicio y al final
string trim(const string& s) {
    const char* blancos = " \t\r\n";
    size_t ini = s.find_first_not_of(blancos);
    if (ini == string::npos) return "";
    size_t fin = s.find_last_not_of(blancos);
    return s.substr(ini, fin - ini + 1);
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Lee una linea de la consola y la recorta
string leerLinea(const string& mensaje) {
    string valor;
    cout << mensaje;
    getline(cin, valor);
    return trim(valor);
}

// Lee un entero validando que sea numero completo
int leerEntero(const string& mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) { limpiarBuffer(); return valor; }
        cout << "  >> Entrada invalida. Ingrese un numero entero.\n";
        limpiarBuffer();
    }
}

// Lee un entero dentro de un rango [minimo, maximo]
int leerEnteroEnRango(const string& mensaje, int minimo, int maximo) {
    while (true) {
        int valor = leerEntero(mensaje);
        if (valor >= minimo && valor <= maximo) return valor;
        cout << "  >> Valor fuera del rango permitido (" << minimo << " a " << maximo << ").\n";
    }
}

// Lee un decimal dentro de un rango [minimo, maximo]
double leerDoubleEnRango(const string& mensaje, double minimo, double maximo) {
    while (true) {
        cout << mensaje;
        double valor;
        if (cin >> valor) {
            limpiarBuffer();
            if (valor >= minimo && valor <= maximo) return valor;
            cout << "  >> Valor fuera del rango permitido (" << minimo << " a " << maximo << ").\n";
        } else {
            cout << "  >> Entrada invalida. Ingrese un numero.\n";
            limpiarBuffer();
        }
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
// MODULO 1: ESTUDIANTES
// ==================================================================
void registrarEstudiante() {
    cout << "\n--- Registrar Estudiante ---\n";
    string carnet = leerLinea("Carne: ");
    if (carnet.empty()) { cout << "  >> Error: el carne no puede estar vacio.\n"; return; }
    if (buscarEstudiantePorCarnet(carnet) != nullptr) {
        cout << "  >> Error: ya existe un estudiante con ese carne.\n";
        return;
    }

    Estudiante e;
    e.carnet = carnet;
    e.nombre = leerLinea("Nombre completo: ");
    if (e.nombre.empty()) { cout << "  >> Error: el nombre no puede estar vacio.\n"; return; }
    e.carrera = leerLinea("Carrera: ");
    e.semestre = leerEnteroEnRango("Semestre (" + to_string(SEMESTRE_MINIMO) + " a "
                                   + to_string(SEMESTRE_MAXIMO) + "): ",
                                   SEMESTRE_MINIMO, SEMESTRE_MAXIMO);
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
        try {
            int s = stoi(semestreStr);
            if (s >= SEMESTRE_MINIMO && s <= SEMESTRE_MAXIMO) e->semestre = s;
            else cout << "  >> Semestre fuera de rango, se conserva el anterior.\n";
        } catch (...) {
            cout << "  >> Valor de semestre invalido, se conserva el anterior.\n";
        }
    }
    cout << "  >> Estudiante actualizado correctamente.\n";
}

void mostrarEstudiante(const Estudiante& e) {
    cout << "\nCarne: " << e.carnet << "\nNombre: " << e.nombre
         << "\nCarrera: " << e.carrera << "\nSemestre: " << e.semestre << "\n";
}

void buscarEstudiante() {
    cout << "\n--- Buscar Estudiante ---\n1. Por carne\n2. Por nombre\n";
    int opcion = leerEntero("Opcion: ");
    if (opcion == 1) {
        string carnet = leerLinea("Carne: ");
        Estudiante* e = buscarEstudiantePorCarnet(carnet);
        if (e == nullptr) { cout << "  >> No se encontro ningun estudiante con ese carne.\n"; return; }
        mostrarEstudiante(*e);
    } else if (opcion == 2) {
        string buscado = leerLinea("Nombre (o parte del nombre): ");
        transform(buscado.begin(), buscado.end(), buscado.begin(), ::tolower);
        bool encontrado = false;
        for (const auto& e : estudiantes) {
            string nl = e.nombre;
            transform(nl.begin(), nl.end(), nl.begin(), ::tolower);
            if (nl.find(buscado) != string::npos) {
                mostrarEstudiante(e);
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
// MODULO 2: CURSOS
// ==================================================================
void registrarCurso() {
    cout << "\n--- Registrar Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    if (codigo.empty()) { cout << "  >> Error: el codigo no puede estar vacio.\n"; return; }
    if (buscarCursoPorCodigo(codigo) != nullptr) {
        cout << "  >> Error: ya existe un curso con ese codigo.\n";
        return;
    }

    Curso c;
    c.codigo = codigo;
    c.nombre = leerLinea("Nombre del curso: ");
    if (c.nombre.empty()) { cout << "  >> Error: el nombre no puede estar vacio.\n"; return; }
    c.creditos = leerEnteroEnRango("Creditos (" + to_string(CREDITOS_MINIMOS) + " a "
                                   + to_string(CREDITOS_MAXIMOS) + "): ",
                                   CREDITOS_MINIMOS, CREDITOS_MAXIMOS);
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
        try {
            int cr = stoi(creditosStr);
            if (cr >= CREDITOS_MINIMOS && cr <= CREDITOS_MAXIMOS) c->creditos = cr;
            else cout << "  >> Creditos fuera de rango, se conservan los anteriores.\n";
        } catch (...) {
            cout << "  >> Valor de creditos invalido, se conserva el anterior.\n";
        }
    }
    cout << "  >> Curso actualizado correctamente.\n";
}

void buscarCurso() {
    cout << "\n--- Buscar Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) { cout << "  >> No se encontro ningun curso con ese codigo.\n"; return; }
    cout << "\nCodigo: " << c->codigo << "\nNombre: " << c->nombre
         << "\nCreditos: " << c->creditos << "\n";
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
// MODULO 3: NOTAS
// ==================================================================
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

    double valorNota = leerDoubleEnRango("Nota (" + to_string((int)NOTA_MINIMA) + " a "
                                         + to_string((int)NOTA_MAXIMA) + "): ",
                                         NOTA_MINIMA, NOTA_MAXIMA);

    for (auto& n : notas) {
        if (n.carnetEstudiante == carnet && n.codigoCurso == codigoCurso) {
            n.calificacion = valorNota;
            cout << "  >> Ya existia una nota para ese estudiante en ese curso; se actualizo el valor.\n";
            return;
        }
    }
    Nota n;
    n.carnetEstudiante = carnet;
    n.codigoCurso = codigoCurso;
    n.calificacion = valorNota;
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
// MODULO 4: PROCESAMIENTO (RECURSIVO)
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

// Cuenta recursivamente cuantas notas de todo el sistema son aprobatorias
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
// MODULO 5: REPORTES
// ==================================================================
void generarReportePromedioEstudiante() {
    cout << "\n--- REPORTE: PROMEDIO POR ESTUDIANTE ---\n";
    if (estudiantes.empty()) { cout << "No hay estudiantes registrados.\n"; return; }
    for (const auto& e : estudiantes) {
        int cant = contarNotasRecursivo(e.carnet, 0);
        double prom = calcularPromedioEstudiante(e.carnet);
        cout << "Carne: " << e.carnet << " | " << e.nombre
             << " | Notas: " << cant
             << " | Promedio: " << fixed << setprecision(2) << prom
             << " | Estado: " << (cant > 0 ? determinarEstado(prom) : string("SIN NOTAS")) << "\n";
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

    string mejorNombre;
    double mayorPromedio = -1.0;
    bool alguno = false;
    for (const auto& e : estudiantes) {
        if (contarNotasRecursivo(e.carnet, 0) == 0) continue;
        double prom = calcularPromedioEstudiante(e.carnet);
        if (!alguno || prom > mayorPromedio) {
            mayorPromedio = prom;
            mejorNombre = e.nombre + " (" + e.carnet + ")";
            alguno = true;
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
// PERSISTENCIA EN ARCHIVO DE TEXTO
// ==================================================================

// Lee una linea de archivo quitando el '\r' que deja Windows
string leerLineaArchivo(ifstream& in) {
    string s;
    getline(in, s);
    if (!s.empty() && s.back() == '\r') s.pop_back();
    return s;
}

void guardarDatos(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "  >> Error al abrir el archivo para guardar.\n";
        return;
    }

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

// Carga el archivo completo en memoria. Si el archivo esta danado,
// no modifica los datos actuales.
void cargarDatos(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "  >> No se encontro \"" << nombreArchivo << "\"; se inicia con datos vacios.\n";
        return;
    }

    vector<Estudiante> estTemp;
    vector<Curso> curTemp;
    vector<Nota> notTemp;

    try {
        size_t totalEst = stoul(leerLineaArchivo(archivo));
        for (size_t i = 0; i < totalEst; i++) {
            Estudiante e;
            e.carnet = leerLineaArchivo(archivo);
            e.nombre = leerLineaArchivo(archivo);
            e.carrera = leerLineaArchivo(archivo);
            e.semestre = stoi(leerLineaArchivo(archivo));
            estTemp.push_back(e);
        }

        size_t totalCur = stoul(leerLineaArchivo(archivo));
        for (size_t i = 0; i < totalCur; i++) {
            Curso c;
            c.codigo = leerLineaArchivo(archivo);
            c.nombre = leerLineaArchivo(archivo);
            c.creditos = stoi(leerLineaArchivo(archivo));
            curTemp.push_back(c);
        }

        size_t totalNot = stoul(leerLineaArchivo(archivo));
        for (size_t i = 0; i < totalNot; i++) {
            Nota n;
            n.carnetEstudiante = leerLineaArchivo(archivo);
            n.codigoCurso = leerLineaArchivo(archivo);
            n.calificacion = stod(leerLineaArchivo(archivo));
            notTemp.push_back(n);
        }
    } catch (const exception&) {
        cout << "  >> Error: el archivo \"" << nombreArchivo
             << "\" esta danado o tiene otro formato. No se cargaron datos.\n";
        return;
    }

    archivo.close();
    estudiantes = estTemp;
    cursos = curTemp;
    notas = notTemp;
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

// ==================================================================
// PROGRAMA PRINCIPAL
// ==================================================================
int main() {
    cargarDatos(ARCHIVO_DATOS);   // carga automatica al iniciar

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
             << "0. Salir (guarda automaticamente)\n";
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1: menuEstudiantes(); break;
            case 2: menuCursosNotas(); break;
            case 3: menuReportes(); break;
            case 4: guardarDatos(ARCHIVO_DATOS); break;
            case 5: cargarDatos(ARCHIVO_DATOS); break;
            case 0:
                guardarDatos(ARCHIVO_DATOS);   // guardado automatico al salir
                cout << "Saliendo del sistema...\n";
                break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
    return 0;
}
