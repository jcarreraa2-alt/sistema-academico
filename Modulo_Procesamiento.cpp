#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

// ==================== ESTRUCTURAS ====================
struct Estudiante {
    int carne;
    string nombre;
    string apellido;
};

struct Curso {
    string codigo;
    string nombre;
    float notaMin;
    float notaMax;
};

struct RegistroNota {
    int carneEstudiante;
    string codigoCurso;
    float calificacion;
};

// ==================== CONSTANTES ====================
const int MAX_EST = 50;
const int MAX_CUR = 20;
const int MAX_NOT = 200;
const float NOTA_APROBACION = 61.0f;  // Criterio de aprobación

// ==================== VARIABLES GLOBALES ====================
Estudiante estudiantes[MAX_EST];
Curso cursos[MAX_CUR];
RegistroNota notas[MAX_NOT];
int numEstudiantes = 0;
int numCursos = 0;
int numNotas = 0;

// Estadísticas globales
int totalAprobados = 0;
int totalReprobados = 0;
float promedioGeneral = 0.0f;
int estudianteMayorPromedio = -1;

// ==================== FUNCIONES AUXILIARES ====================

int buscarEstudiante(int carne) {
    for (int i = 0; i < numEstudiantes; i++) {
        if (estudiantes[i].carne == carne) return i;
    }
    return -1;
}

int contarNotasEstudiante(int carne) {
    int cont = 0;
    for (int i = 0; i < numNotas; i++) {
        if (notas[i].carneEstudiante == carne) cont++;
    }
    return cont;
}

// ==================== RECURSIVIDAD (requerida por el proyecto) ====================

// Suma recursiva de las calificaciones de un estudiante
float sumarCalificacionesRecursivo(int carne, int indice) {
    if (indice >= numNotas) return 0.0f;  // Caso base

    if (notas[indice].carneEstudiante == carne) {
        return notas[indice].calificacion + sumarCalificacionesRecursivo(carne, indice + 1);
    }
    return sumarCalificacionesRecursivo(carne, indice + 1);
}

// Contar aprobados de forma recursiva
int contarAprobadosRecursivo(int indice) {
    if (indice >= numNotas) return 0;

    int cont = 0;
    if (notas[indice].calificacion >= NOTA_APROBACION) cont = 1;

    return cont + contarAprobadosRecursivo(indice + 1);
}

// ==================== FUNCIONES DEL MÓDULO DE PROCESAMIENTO ====================

// 1. Calcular promedio por estudiante (usa recursión)
float calcularPromedioEstudiante(int carne) {
    int cantidad = contarNotasEstudiante(carne);
    if (cantidad == 0) return 0.0f;

    float suma = sumarCalificacionesRecursivo(carne, 0);
    return suma / cantidad;
}

// 2. Determinar estado (Aprobado / Reprobado)
string determinarEstado(float promedio) {
    if (promedio >= NOTA_APROBACION)
        return "APROBADO";
    else
        return "REPROBADO";
}

// 3. Actualizar estadísticas académicas
void actualizarEstadisticas() {
    totalAprobados = 0;
    totalReprobados = 0;
    float sumaPromedios = 0.0f;
    int estudiantesConNotas = 0;
    float mayorPromedio = -1.0f;
    estudianteMayorPromedio = -1;

    for (int i = 0; i < numEstudiantes; i++) {
        float prom = calcularPromedioEstudiante(estudiantes[i].carne);

        if (contarNotasEstudiante(estudiantes[i].carne) > 0) {
            estudiantesConNotas++;
            sumaPromedios += prom;

            if (prom >= NOTA_APROBACION)
                totalAprobados++;
            else
                totalReprobados++;

            if (prom > mayorPromedio) {
                mayorPromedio = prom;
                estudianteMayorPromedio = i;
            }
        }
    }

    if (estudiantesConNotas > 0)
        promedioGeneral = sumaPromedios / estudiantesConNotas;
    else
        promedioGeneral = 0.0f;
}

// ==================== CARGA DE DATOS DE EJEMPLO ====================
void cargarDatosEjemplo() {
    // Estudiantes
    estudiantes[0] = {2021001, "Ana", "Lopez"};
    estudiantes[1] = {2021002, "Carlos", "Perez"};
    estudiantes[2] = {2021003, "Maria", "Garcia"};
    estudiantes[3] = {2021004, "Luis", "Martinez"};
    numEstudiantes = 4;

    // Cursos
    cursos[0] = {"MAT101", "Matematicas I", 0, 100};
    cursos[1] = {"PRG101", "Programacion I", 0, 100};
    cursos[2] = {"FIS101", "Fisica I", 0, 100};
    numCursos = 3;

    // Notas
    notas[0] = {2021001, "MAT101", 85.5f};
    notas[1] = {2021001, "PRG101", 90.0f};
    notas[2] = {2021001, "FIS101", 78.0f};

    notas[3] = {2021002, "MAT101", 55.0f};
    notas[4] = {2021002, "PRG101", 62.0f};

    notas[5] = {2021003, "MAT101", 95.0f};
    notas[6] = {2021003, "PRG101", 88.5f};
    notas[7] = {2021003, "FIS101", 92.0f};

    notas[8] = {2021004, "MAT101", 45.0f};
    notas[9] = {2021004, "PRG101", 50.0f};
    numNotas = 10;
}

// ==================== MOSTRAR RESULTADOS ====================
void mostrarResultadosProcesamiento() {
    cout << "\n=====================================================\n";
    cout << "   MODULO DE PROCESAMIENTO - RESULTADOS\n";
    cout << "=====================================================\n\n";

    cout << fixed << setprecision(2);

    cout << "--- PROMEDIOS POR ESTUDIANTE ---\n";
    for (int i = 0; i < numEstudiantes; i++) {
        float prom = calcularPromedioEstudiante(estudiantes[i].carne);
        string estado = determinarEstado(prom);
        int cant = contarNotasEstudiante(estudiantes[i].carne);

        cout << "Carne: " << estudiantes[i].carne
             << " | " << estudiantes[i].nombre << " " << estudiantes[i].apellido
             << " | Notas: " << cant
             << " | Promedio: " << prom
             << " | Estado: " << estado << endl;
    }

    cout << "\n--- ESTADISTICAS ACADEMICAS ---\n";
    cout << "Total de estudiantes con notas : " << (totalAprobados + totalReprobados) << endl;
    cout << "Aprobados                      : " << totalAprobados << endl;
    cout << "Reprobados                     : " << totalReprobados << endl;
    cout << "Promedio general del sistema   : " << promedioGeneral << endl;

    if (estudianteMayorPromedio != -1) {
        float mayor = calcularPromedioEstudiante(estudiantes[estudianteMayorPromedio].carne);
        cout << "Estudiante con mayor promedio  : "
             << estudiantes[estudianteMayorPromedio].nombre << " "
             << estudiantes[estudianteMayorPromedio].apellido
             << " (" << mayor << ")" << endl;
    }

    cout << "\n--- DETALLE DE NOTAS (para verificacion) ---\n";
    for (int i = 0; i < numNotas; i++) {
        cout << "  Carne " << notas[i].carneEstudiante
             << " | Curso " << notas[i].codigoCurso
             << " | Nota " << notas[i].calificacion << endl;
    }

    cout << "\n=====================================================\n";
}

// ==================== MAIN ====================
int main() {
    cout << "=====================================================\n";
    cout << "  SISTEMA DE CONTROL ACADEMICO\n";
    cout << "  MODULO DE PROCESAMIENTO\n";
    cout << "  Universidad Mariano Galvez de Guatemala\n";
    cout << "=====================================================\n";

    // Cargar datos de ejemplo
    cargarDatosEjemplo();

    cout << "\nDatos cargados correctamente.\n";
    cout << "Estudiantes: " << numEstudiantes << endl;
    cout << "Cursos     : " << numCursos << endl;
    cout << "Notas      : " << numNotas << endl;

    // ===== PROCESAMIENTO =====
    cout << "\n>>> Ejecutando procesamiento...\n";
    actualizarEstadisticas();

    // Mostrar resultados
    mostrarResultadosProcesamiento();

    cout << "\nProcesamiento finalizado correctamente.\n";
    return 0;
}