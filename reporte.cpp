#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct NotaCurso {
    string codigoCurso;
    string nombreCurso;
    float calificacion;
    bool aprobado;
};

struct Estudiante {
    string carne;
    string nombre;
    vector<NotaCurso> notas;
};

// Declaración de funciones auxiliares y de reportes
int buscarEstudiante(const vector<Estudiante>& estudiantes, const string& carneBuscado);
void generarReportePromedioEstudiante(const vector<Estudiante>& estudiantes);
void generarReportePromedioCurso(const vector<Estudiante>& estudiantes);
void generarReporteAprobadosReprobados(const vector<Estudiante>& estudiantes);
void generarReporteMejorEstudiante(const vector<Estudiante>& estudiantes);
void generarExpedientePorCarne(const vector<Estudiante>& estudiantes);
void guardarDatos(const vector<Estudiante>& estudiantes, const string& nombreArchivo);
void cargarDatos(vector<Estudiante>& estudiantes, const string& nombreArchivo);

int main() {
    vector<Estudiante> estudiantes;
    cargarDatos(estudiantes, "datos_academicos.txt");

    int opcion = 0;
    do {
        cout << "\n========================================\n";
        cout << "   MODULO DE REPORTES / PERSISTENCIA    \n";
        cout << "========================================\n";
        cout << "1. Promedio por estudiante\n";
        cout << "2. Promedio por curso\n";
        cout << "3. Cantidad de aprobados y reprobados\n";
        cout << "4. Estudiantes con mayor promedio\n";
        cout << "5. Expediente por carne\n";
        cout << "6. Guardar datos actuales en archivo\n";
        cout << "7. Salir\n";
        cout << "Seleccione una opcion: ";

        // Validación robusta de la entrada del menú (evita bucle infinito si ingresan letras)
        if (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor ingrese un numero.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            opcion = 0;
            continue;
        }

        switch (opcion) {
            case 1:
                generarReportePromedioEstudiante(estudiantes);
                break;
            case 2:
                generarReportePromedioCurso(estudiantes);
                break;
            case 3:
                generarReporteAprobadosReprobados(estudiantes);
                break;
            case 4:
                generarReporteMejorEstudiante(estudiantes);
                break;
            case 5:
                generarExpedientePorCarne(estudiantes);
                break;
            case 6:
                guardarDatos(estudiantes, "datos_academicos.txt");
                cout << "Datos guardados exitosamente.\n";
                break;
            case 7:
                cout << "Saliendo del modulo...\n";
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 7);

    return 0;
}

// Función auxiliar para buscar un estudiante y retornar su índice (o -1 si no existe)
int buscarEstudiante(const vector<Estudiante>& estudiantes, const string& carneBuscado) {
    for (size_t i = 0; i < estudiantes.size(); i++) {
        if (estudiantes[i].carne == carneBuscado) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void generarReportePromedioEstudiante(const vector<Estudiante>& estudiantes) {
    cout << "\n--- REPORTE: PROMEDIO POR ESTUDIANTE ---\n";
    if (estudiantes.empty()) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    for (const auto& est : estudiantes) {
        if (est.notas.empty()) {
            cout << "Carne: " << est.carne << " | Nombre: " << est.nombre << " | Sin notas registradas.\n";
            continue;
        }
        float suma = 0;
        for (const auto& nota : est.notas) {
            suma += nota.calificacion;
        }
        float promedio = suma / est.notas.size();
        cout << "Carne: " << est.carne << " | Nombre: " << est.nombre << " | Promedio: " << fixed << setprecision(2) << promedio << "\n";
    }
}

void generarReportePromedioCurso(const vector<Estudiante>& estudiantes) {
    cout << "\n--- REPORTE: PROMEDIO POR CURSO ---\n";
    string cursoBuscado;
    cout << "Ingrese el codigo del curso a consultar: ";
    cin >> cursoBuscado;

    float sumaNotas = 0;
    int contador = 0;

    for (const auto& est : estudiantes) {
        for (const auto& nota : est.notas) {
            if (nota.codigoCurso == cursoBuscado) {
                sumaNotas += nota.calificacion;
                contador++;
            }
        }
    }

    if (contador > 0) {
        cout << "Curso: " << cursoBuscado << " | Promedio General: " << fixed << setprecision(2) << (sumaNotas / contador) << " (Total de alumnos evaluados: " << contador << ")\n";
    } else {
        cout << "No se encontraron registros para el curso ingresado.\n";
    }
}

void generarReporteAprobadosReprobados(const vector<Estudiante>& estudiantes) {
    cout << "\n--- REPORTE: APROBADOS Y REPROBADOS ---\n";
    int aprobados = 0;
    int reprobados = 0;

    for (const auto& est : estudiantes) {
        for (const auto& nota : est.notas) {
            // Se utiliza directamente el campo booleano 'aprobado'
            if (nota.aprobado) {
                aprobados++;
            } else {
                reprobados++;
            }
        }
    }

    cout << "Total de asignaciones aprobadas: " << aprobados << "\n";
    cout << "Total de asignaciones reprobadas: " << reprobados << "\n";
}

void generarReporteMejorEstudiante(const vector<Estudiante>& estudiantes) {
    cout << "\n--- REPORTE: ESTUDIANTE CON MAYOR PROMEDIO ---\n";
    if (estudiantes.empty()) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    string mejorEstudiante = "";
    float mayorPromedio = -1.0f;

    for (const auto& est : estudiantes) {
        if (est.notas.empty()) continue; // Evita división por cero
        float suma = 0;
        for (const auto& nota : est.notas) {
            suma += nota.calificacion;
        }
        float promedio = suma / est.notas.size();
        if (promedio > mayorPromedio) {
            mayorPromedio = promedio;
            mejorEstudiante = est.nombre + " (" + est.carne + ")";
        }
    }

    if (mayorPromedio != -1.0f) {
        cout << "El estudiante con mayor promedio es: " << mejorEstudiante << " con " << fixed << setprecision(2) << mayorPromedio << "\n";
    } else {
        cout << "No hay suficientes datos de notas.\n";
    }
}

void generarExpedientePorCarne(const vector<Estudiante>& estudiantes) {
    cout << "\n--- REPORTE: EXPEDIENTE POR CARNE ---\n";
    string carneBuscado;
    cout << "Ingrese el numero de carne del estudiante: ";
    cin >> carneBuscado;

    int index = buscarEstudiante(estudiantes, carneBuscado);
    if (index != -1) {
        const auto& est = estudiantes[index];
        cout << "\n========================================\n";
        cout << "EXPEDIENTE ACADEMICO\n";
        cout << "Carne: " << est.carne << "\n";
        cout << "Nombre: " << est.nombre << "\n";
        cout << "Historial de Cursos:\n";
        if (est.notas.empty()) {
            cout << " - El estudiante no tiene cursos asignados.\n";
        } else {
            for (const auto& nota : est.notas) {
                cout << " - [" << nota.codigoCurso << "] " << nota.nombreCurso 
                     << " | Nota: " << fixed << setprecision(2) << nota.calificacion 
                     << " | " << (nota.aprobado ? "Aprobado" : "Reprobado") << "\n";
            }
        }
        cout << "========================================\n";
    } else {
        cout << "Estudiante con carne " << carneBuscado << " no encontrado.\n";
    }
}

void guardarDatos(const vector<Estudiante>& estudiantes, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo para guardar.\n";
        return;
    }

    archivo << estudiantes.size() << "\n";
    for (const auto& est : estudiantes) {
        archivo << est.carne << "\n" << est.nombre << "\n";
        archivo << est.notas.size() << "\n";
        for (const auto& nota : est.notas) {
            archivo << nota.codigoCurso << "\n" << nota.nombreCurso << "\n" << nota.calificacion << "\n";
        }
    }
    archivo.close();
}

void cargarDatos(vector<Estudiante>& estudiantes, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        Estudiante estPrueba = {"2026001", "Juan Perez", {{"CS101", "Programacion I", 85.5f, true}, {"MAT101", "Matematica Discreta", 75.0f, true}}};
        estudiantes.push_back(estPrueba);
        return;
    }

    int totalEstudiantes;
    if (!(archivo >> totalEstudiantes)) return;
    estudiantes.clear();

    for (int i = 0; i < totalEstudiantes; i++) {
        Estudiante est;
        archivo >> ws;
        getline(archivo, est.carne);
        getline(archivo, est.nombre);
        
        int totalNotas;
        archivo >> totalNotas;
        for (int j = 0; j < totalNotas; j++) {
            NotaCurso nota;
            archivo >> ws;
            getline(archivo, nota.codigoCurso);
            getline(archivo, nota.nombreCurso);
            archivo >> nota.calificacion;
            nota.aprobado = (nota.calificacion >= 61.0f); // Se define el estado aprobado al cargar
            est.notas.push_back(nota);
        }
        estudiantes.push_back(est);
    }
    archivo.close();
}