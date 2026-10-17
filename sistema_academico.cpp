/*
 * Sistema Académico - Módulo de Estudiantes y Cursos/Notas
 * -----------------------------------------------------------
 * Programa de consola en C++ (sin base de datos, sin interfaz gráfica).
 * Toda la información se mantiene en memoria mientras el programa corre,
 * usando estructuras y vectores de la STL.
 *
 * Funcionalidades:
 *  Estudiantes:
 *    - Registrar
 *    - Editar
 *    - Buscar (por carné o por nombre)
 *    - Consultar / Listar todos
 *  Cursos:
 *    - Registrar
 *    - Editar
 *    - Buscar
 *    - Consultar / Listar todos
 *  Notas:
 *    - Registrar (valida que el estudiante y el curso existan,
 *                 y que la nota esté dentro del rango permitido)
 *    - Consultar notas por estudiante
 *    - Consultar notas por curso
 *
 * Compilar:
 *    g++ -std=c++17 -O2 -o sistema_academico sistema_academico.cpp
 *
 * Ejecutar:
 *    ./sistema_academico
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>

// ----------------------------------------------------------------
// Constantes de configuración
// ----------------------------------------------------------------
const double NOTA_MINIMA = 0.0;
const double NOTA_MAXIMA = 100.0;

// ----------------------------------------------------------------
// Estructuras de datos
// ----------------------------------------------------------------
struct Estudiante {
    std::string carnet;
    std::string nombre;
    std::string carrera;
    int semestre;
};

struct Curso {
    std::string codigo;
    std::string nombre;
    int creditos;
};

struct Nota {
    std::string carnetEstudiante;
    std::string codigoCurso;
    double nota;
};

// ----------------------------------------------------------------
// Almacenamiento en memoria (sustituye a la base de datos)
// ----------------------------------------------------------------
std::vector<Estudiante> estudiantes;
std::vector<Curso> cursos;
std::vector<Nota> notas;

// ----------------------------------------------------------------
// Utilidades de entrada de datos
// ----------------------------------------------------------------
void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string leerLinea(const std::string& mensaje) {
    std::string valor;
    std::cout << mensaje;
    std::getline(std::cin, valor);
    return valor;
}

int leerEntero(const std::string& mensaje) {
    int valor;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) {
            limpiarBuffer();
            return valor;
        }
        std::cout << "  >> Entrada invalida. Ingrese un numero entero.\n";
        limpiarBuffer();
    }
}

double leerDouble(const std::string& mensaje) {
    double valor;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) {
            limpiarBuffer();
            return valor;
        }
        std::cout << "  >> Entrada invalida. Ingrese un numero.\n";
        limpiarBuffer();
    }
}

// ----------------------------------------------------------------
// Funciones de búsqueda (devuelven puntero o nullptr si no existe)
// ----------------------------------------------------------------
Estudiante* buscarEstudiantePorCarnet(const std::string& carnet) {
    for (auto& e : estudiantes) {
        if (e.carnet == carnet) return &e;
    }
    return nullptr;
}

Curso* buscarCursoPorCodigo(const std::string& codigo) {
    for (auto& c : cursos) {
        if (c.codigo == codigo) return &c;
    }
    return nullptr;
}

// ----------------------------------------------------------------
// MÓDULO: ESTUDIANTES
// ----------------------------------------------------------------
void registrarEstudiante() {
    std::cout << "\n--- Registrar Estudiante ---\n";
    std::string carnet = leerLinea("Carne: ");

    if (buscarEstudiantePorCarnet(carnet) != nullptr) {
        std::cout << "  >> Error: ya existe un estudiante con ese carne.\n";
        return;
    }

    Estudiante e;
    e.carnet = carnet;
    e.nombre = leerLinea("Nombre completo: ");
    e.carrera = leerLinea("Carrera: ");
    e.semestre = leerEntero("Semestre: ");

    estudiantes.push_back(e);
    std::cout << "  >> Estudiante registrado correctamente.\n";
}

void editarEstudiante() {
    std::cout << "\n--- Editar Estudiante ---\n";
    std::string carnet = leerLinea("Carne del estudiante a editar: ");

    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) {
        std::cout << "  >> Error: no existe un estudiante con ese carne.\n";
        return;
    }

    std::cout << "Deje en blanco un campo para conservar el valor actual.\n";

    std::string nombre = leerLinea("Nombre [" + e->nombre + "]: ");
    if (!nombre.empty()) e->nombre = nombre;

    std::string carrera = leerLinea("Carrera [" + e->carrera + "]: ");
    if (!carrera.empty()) e->carrera = carrera;

    std::string semestreStr = leerLinea("Semestre [" + std::to_string(e->semestre) + "]: ");
    if (!semestreStr.empty()) {
        try {
            e->semestre = std::stoi(semestreStr);
        } catch (...) {
            std::cout << "  >> Valor de semestre invalido, se conserva el anterior.\n";
        }
    }

    std::cout << "  >> Estudiante actualizado correctamente.\n";
}

void buscarEstudiante() {
    std::cout << "\n--- Buscar Estudiante ---\n";
    std::cout << "1. Por carne\n2. Por nombre\n";
    int opcion = leerEntero("Opcion: ");

    if (opcion == 1) {
        std::string carnet = leerLinea("Carne: ");
        Estudiante* e = buscarEstudiantePorCarnet(carnet);
        if (e == nullptr) {
            std::cout << "  >> No se encontro ningun estudiante con ese carne.\n";
            return;
        }
        std::cout << "\nCarne: " << e->carnet
                  << "\nNombre: " << e->nombre
                  << "\nCarrera: " << e->carrera
                  << "\nSemestre: " << e->semestre << "\n";
    } else if (opcion == 2) {
        std::string nombre = leerLinea("Nombre (o parte del nombre): ");
        std::string nombreLower = nombre;
        std::transform(nombreLower.begin(), nombreLower.end(), nombreLower.begin(), ::tolower);

        bool encontrado = false;
        for (const auto& e : estudiantes) {
            std::string nombreEstLower = e.nombre;
            std::transform(nombreEstLower.begin(), nombreEstLower.end(), nombreEstLower.begin(), ::tolower);
            if (nombreEstLower.find(nombreLower) != std::string::npos) {
                std::cout << "\nCarne: " << e.carnet
                          << "\nNombre: " << e.nombre
                          << "\nCarrera: " << e.carrera
                          << "\nSemestre: " << e.semestre << "\n";
                encontrado = true;
            }
        }
        if (!encontrado) std::cout << "  >> No se encontraron coincidencias.\n";
    } else {
        std::cout << "  >> Opcion invalida.\n";
    }
}

void listarEstudiantes() {
    std::cout << "\n--- Listado de Estudiantes ---\n";
    if (estudiantes.empty()) {
        std::cout << "  (No hay estudiantes registrados)\n";
        return;
    }
    std::cout << std::left
              << std::setw(12) << "Carne"
              << std::setw(28) << "Nombre"
              << std::setw(20) << "Carrera"
              << std::setw(10) << "Semestre" << "\n";
    std::cout << std::string(70, '-') << "\n";
    for (const auto& e : estudiantes) {
        std::cout << std::left
                  << std::setw(12) << e.carnet
                  << std::setw(28) << e.nombre
                  << std::setw(20) << e.carrera
                  << std::setw(10) << e.semestre << "\n";
    }
}

// ----------------------------------------------------------------
// MÓDULO: CURSOS
// ----------------------------------------------------------------
void registrarCurso() {
    std::cout << "\n--- Registrar Curso ---\n";
    std::string codigo = leerLinea("Codigo del curso: ");

    if (buscarCursoPorCodigo(codigo) != nullptr) {
        std::cout << "  >> Error: ya existe un curso con ese codigo.\n";
        return;
    }

    Curso c;
    c.codigo = codigo;
    c.nombre = leerLinea("Nombre del curso: ");
    c.creditos = leerEntero("Creditos: ");

    cursos.push_back(c);
    std::cout << "  >> Curso registrado correctamente.\n";
}

void editarCurso() {
    std::cout << "\n--- Editar Curso ---\n";
    std::string codigo = leerLinea("Codigo del curso a editar: ");

    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) {
        std::cout << "  >> Error: no existe un curso con ese codigo.\n";
        return;
    }

    std::cout << "Deje en blanco un campo para conservar el valor actual.\n";

    std::string nombre = leerLinea("Nombre [" + c->nombre + "]: ");
    if (!nombre.empty()) c->nombre = nombre;

    std::string creditosStr = leerLinea("Creditos [" + std::to_string(c->creditos) + "]: ");
    if (!creditosStr.empty()) {
        try {
            c->creditos = std::stoi(creditosStr);
        } catch (...) {
            std::cout << "  >> Valor de creditos invalido, se conserva el anterior.\n";
        }
    }

    std::cout << "  >> Curso actualizado correctamente.\n";
}

void buscarCurso() {
    std::cout << "\n--- Buscar Curso ---\n";
    std::string codigo = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) {
        std::cout << "  >> No se encontro ningun curso con ese codigo.\n";
        return;
    }
    std::cout << "\nCodigo: " << c->codigo
              << "\nNombre: " << c->nombre
              << "\nCreditos: " << c->creditos << "\n";
}

void listarCursos() {
    std::cout << "\n--- Listado de Cursos ---\n";
    if (cursos.empty()) {
        std::cout << "  (No hay cursos registrados)\n";
        return;
    }
    std::cout << std::left
              << std::setw(12) << "Codigo"
              << std::setw(30) << "Nombre"
              << std::setw(10) << "Creditos" << "\n";
    std::cout << std::string(52, '-') << "\n";
    for (const auto& c : cursos) {
        std::cout << std::left
                  << std::setw(12) << c.codigo
                  << std::setw(30) << c.nombre
                  << std::setw(10) << c.creditos << "\n";
    }
}

// ----------------------------------------------------------------
// MÓDULO: NOTAS
// ----------------------------------------------------------------
void registrarNota() {
    std::cout << "\n--- Registrar Nota ---\n";
    std::string carnet = leerLinea("Carne del estudiante: ");
    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) {
        std::cout << "  >> Error: no existe ningun estudiante con ese carne. "
                     "No se puede registrar la nota.\n";
        return;
    }

    std::string codigoCurso = leerLinea("Codigo del curso: ");
    Curso* c = buscarCursoPorCodigo(codigoCurso);
    if (c == nullptr) {
        std::cout << "  >> Error: no existe ningun curso con ese codigo. "
                     "No se puede registrar la nota.\n";
        return;
    }

    double valorNota = leerDouble("Nota (" + std::to_string((int)NOTA_MINIMA) +
                                   " a " + std::to_string((int)NOTA_MAXIMA) + "): ");
    if (valorNota < NOTA_MINIMA || valorNota > NOTA_MAXIMA) {
        std::cout << "  >> Error: la nota esta fuera del rango permitido ("
                  << NOTA_MINIMA << " - " << NOTA_MAXIMA << "). No se registro.\n";
        return;
    }

    // Si ya existe una nota para ese estudiante en ese curso, se actualiza en vez de duplicar
    for (auto& n : notas) {
        if (n.carnetEstudiante == carnet && n.codigoCurso == codigoCurso) {
            n.nota = valorNota;
            std::cout << "  >> Ya existia una nota para ese estudiante en ese curso; "
                         "se actualizo el valor.\n";
            return;
        }
    }

    Nota n;
    n.carnetEstudiante = carnet;
    n.codigoCurso = codigoCurso;
    n.nota = valorNota;
    notas.push_back(n);

    std::cout << "  >> Nota registrada correctamente.\n";
}

void consultarNotasPorEstudiante() {
    std::cout << "\n--- Consultar Notas por Estudiante ---\n";
    std::string carnet = leerLinea("Carne del estudiante: ");

    Estudiante* e = buscarEstudiantePorCarnet(carnet);
    if (e == nullptr) {
        std::cout << "  >> Error: no existe ningun estudiante con ese carne.\n";
        return;
    }

    std::cout << "\nNotas de " << e->nombre << " (" << e->carnet << "):\n";
    bool encontrado = false;
    for (const auto& n : notas) {
        if (n.carnetEstudiante == carnet) {
            Curso* c = buscarCursoPorCodigo(n.codigoCurso);
            std::string nombreCurso = (c != nullptr) ? c->nombre : "(curso no encontrado)";
            std::cout << "  - " << n.codigoCurso << " " << nombreCurso
                      << ": " << std::fixed << std::setprecision(2) << n.nota << "\n";
            encontrado = true;
        }
    }
    if (!encontrado) std::cout << "  (Este estudiante no tiene notas registradas)\n";
}

void consultarNotasPorCurso() {
    std::cout << "\n--- Consultar Notas por Curso ---\n";
    std::string codigo = leerLinea("Codigo del curso: ");

    Curso* c = buscarCursoPorCodigo(codigo);
    if (c == nullptr) {
        std::cout << "  >> Error: no existe ningun curso con ese codigo.\n";
        return;
    }

    std::cout << "\nNotas del curso " << c->nombre << " (" << c->codigo << "):\n";
    bool encontrado = false;
    for (const auto& n : notas) {
        if (n.codigoCurso == codigo) {
            Estudiante* e = buscarEstudiantePorCarnet(n.carnetEstudiante);
            std::string nombreEst = (e != nullptr) ? e->nombre : "(estudiante no encontrado)";
            std::cout << "  - " << n.carnetEstudiante << " " << nombreEst
                      << ": " << std::fixed << std::setprecision(2) << n.nota << "\n";
            encontrado = true;
        }
    }
    if (!encontrado) std::cout << "  (Este curso no tiene notas registradas)\n";
}

// ----------------------------------------------------------------
// MENÚS
// ----------------------------------------------------------------
void menuEstudiantes() {
    int opcion;
    do {
        std::cout << "\n===== MENU ESTUDIANTES =====\n"
                  << "1. Registrar estudiante\n"
                  << "2. Editar estudiante\n"
                  << "3. Buscar estudiante\n"
                  << "4. Consultar / listar todos\n"
                  << "0. Volver al menu principal\n";
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: registrarEstudiante(); break;
            case 2: editarEstudiante(); break;
            case 3: buscarEstudiante(); break;
            case 4: listarEstudiantes(); break;
            case 0: break;
            default: std::cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void menuCursosNotas() {
    int opcion;
    do {
        std::cout << "\n===== MENU CURSOS / NOTAS =====\n"
                  << "1. Registrar curso\n"
                  << "2. Editar curso\n"
                  << "3. Buscar curso\n"
                  << "4. Listar cursos\n"
                  << "5. Registrar nota\n"
                  << "6. Consultar notas por estudiante\n"
                  << "7. Consultar notas por curso\n"
                  << "0. Volver al menu principal\n";
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
            default: std::cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

int main() {
    int opcion;
    do {
        std::cout << "\n=========================================\n"
                  << "   SISTEMA ACADEMICO - MENU PRINCIPAL\n"
                  << "=========================================\n"
                  << "1. Modulo Estudiantes\n"
                  << "2. Modulo Cursos / Notas\n"
                  << "0. Salir\n";
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: menuEstudiantes(); break;
            case 2: menuCursosNotas(); break;
            case 0: std::cout << "Saliendo del sistema...\n"; break;
            default: std::cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);

    return 0;
}
