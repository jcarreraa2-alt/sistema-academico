/*
 * SISTEMA ACADEMICO - VERSION MEJORADA (UN SOLO ARCHIVO)
 * ============================================================
 * Compilar: g++ -std=c++11 -o sistema_academico sistema_academico_mejorado.cpp
 * Ejecutar: ./sistema_academico
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

// ================================================================
// CLASE ESTUDIANTE
// ================================================================
class Estudiante {
private:
    string carnet;
    string nombre;
    string carrera;
    int semestre;

    bool esCarnetValido(const string& c) const {
        return !c.empty() && c.length() >= 5;
    }

    bool esNombreValido(const string& n) const {
        return !n.empty() && n.length() >= 3;
    }

    bool esSemestreValido(int s) const {
        return s >= 1 && s <= 12;
    }

public:
    Estudiante() : carnet(""), nombre(""), carrera(""), semestre(0) {}
    
    Estudiante(string car, string nom, string carr, int sem) {
        if (esCarnetValido(car)) carnet = car;
        if (esNombreValido(nom)) nombre = nom;
        carrera = carr;
        if (esSemestreValido(sem)) semestre = sem;
    }

    string getCarnet() const { return carnet; }
    string getNombre() const { return nombre; }
    string getCarrera() const { return carrera; }
    int getSemestre() const { return semestre; }

    bool setCarnet(const string& c) {
        if (!esCarnetValido(c)) {
            cout << "  >> Error: El carnet debe tener al menos 5 caracteres.\n";
            return false;
        }
        carnet = c;
        return true;
    }

    bool setNombre(const string& n) {
        if (!esNombreValido(n)) {
            cout << "  >> Error: El nombre debe tener al menos 3 caracteres.\n";
            return false;
        }
        nombre = n;
        return true;
    }

    bool setCarrera(const string& carr) {
        if (carr.empty()) {
            cout << "  >> Error: La carrera no puede estar vacia.\n";
            return false;
        }
        carrera = carr;
        return true;
    }

    bool setSemestre(int s) {
        if (!esSemestreValido(s)) {
            cout << "  >> Error: El semestre debe estar entre 1 y 12.\n";
            return false;
        }
        semestre = s;
        return true;
    }

    void mostrar() const {
        cout << "Carnet: " << carnet << " | Nombre: " << nombre 
             << " | Carrera: " << carrera << " | Semestre: " << semestre << "\n";
    }
};

// ================================================================
// CLASE CURSO
// ================================================================
class Curso {
private:
    string codigo;
    string nombre;
    int creditos;

    bool esCodigoValido(const string& c) const {
        return !c.empty() && c.length() >= 3;
    }

    bool esCreditosValido(int cr) const {
        return cr >= 1 && cr <= 12;
    }

public:
    Curso() : codigo(""), nombre(""), creditos(0) {}
    
    Curso(string cod, string nom, int cred) {
        if (esCodigoValido(cod)) codigo = cod;
        nombre = nom;
        if (esCreditosValido(cred)) creditos = cred;
    }

    string getCodigo() const { return codigo; }
    string getNombre() const { return nombre; }
    int getCreditos() const { return creditos; }

    bool setCodigo(const string& c) {
        if (!esCodigoValido(c)) {
            cout << "  >> Error: El codigo debe tener al menos 3 caracteres.\n";
            return false;
        }
        codigo = c;
        return true;
    }

    bool setNombre(const string& n) {
        if (n.empty()) {
            cout << "  >> Error: El nombre del curso no puede estar vacio.\n";
            return false;
        }
        nombre = n;
        return true;
    }

    bool setCreditos(int cr) {
        if (!esCreditosValido(cr)) {
            cout << "  >> Error: Los creditos deben estar entre 1 y 12.\n";
            return false;
        }
        creditos = cr;
        return true;
    }

    void mostrar() const {
        cout << "Codigo: " << codigo << " | Nombre: " << nombre 
             << " | Creditos: " << creditos << "\n";
    }
};

// ================================================================
// CLASE NOTA
// ================================================================
class Nota {
private:
    string carnetEstudiante;
    string codigoCurso;
    double calificacion;

    bool esCalificacionValida(double cal) const {
        return cal >= 0.0 && cal <= 100.0;
    }

public:
    Nota() : carnetEstudiante(""), codigoCurso(""), calificacion(0.0) {}
    
    Nota(string car, string cod, double cal) {
        carnetEstudiante = car;
        codigoCurso = cod;
        if (esCalificacionValida(cal)) calificacion = cal;
    }

    string getCarnetEstudiante() const { return carnetEstudiante; }
    string getCodigoCurso() const { return codigoCurso; }
    double getCalificacion() const { return calificacion; }
    
    string getEstado() const {
        return (calificacion >= 61.0) ? "Aprobado" : "Reprobado";
    }

    bool setCarnetEstudiante(const string& car) {
        if (car.empty()) {
            cout << "  >> Error: El carnet no puede estar vacio.\n";
            return false;
        }
        carnetEstudiante = car;
        return true;
    }

    bool setCodigoCurso(const string& cod) {
        if (cod.empty()) {
            cout << "  >> Error: El codigo del curso no puede estar vacio.\n";
            return false;
        }
        codigoCurso = cod;
        return true;
    }

    bool setCalificacion(double cal) {
        if (!esCalificacionValida(cal)) {
            cout << "  >> Error: La calificacion debe estar entre 0 y 100.\n";
            return false;
        }
        calificacion = cal;
        return true;
    }

    void mostrar() const {
        cout << "Carnet: " << carnetEstudiante << " | Curso: " << codigoCurso 
             << " | Calificacion: " << fixed << setprecision(2) << calificacion 
             << " | " << getEstado() << "\n";
    }
};

// ================================================================
// GESTORES (Contenedores)
// ================================================================
vector<Estudiante> estudiantes;
vector<Curso> cursos;
vector<Nota> notas;

// ================================================================
// FUNCIONES AUXILIARES
// ================================================================
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
        if (cin >> valor) {
            limpiarBuffer();
            return valor;
        }
        cout << "  >> Error: Ingrese un numero entero valido.\n";
        limpiarBuffer();
    }
}

double leerDouble(const string& mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            limpiarBuffer();
            return valor;
        }
        cout << "  >> Error: Ingrese un numero valido.\n";
        limpiarBuffer();
    }
}

// ================================================================
// BUSQUEDAS
// ================================================================
Estudiante* buscarEstudiantePorCarnet(const string& carnet) {
    for (size_t i = 0; i < estudiantes.size(); i++) {
        if (estudiantes[i].getCarnet() == carnet) {
            return &estudiantes[i];
        }
    }
    return nullptr;
}

Curso* buscarCursoPorCodigo(const string& codigo) {
    for (size_t i = 0; i < cursos.size(); i++) {
        if (cursos[i].getCodigo() == codigo) {
            return &cursos[i];
        }
    }
    return nullptr;
}

Nota* buscarNota(const string& carnet, const string& codigoCurso) {
    for (size_t i = 0; i < notas.size(); i++) {
        if (notas[i].getCarnetEstudiante() == carnet && 
            notas[i].getCodigoCurso() == codigoCurso) {
            return &notas[i];
        }
    }
    return nullptr;
}

// ================================================================
// OPERACIONES CON ESTUDIANTES
// ================================================================
void registrarEstudiante() {
    cout << "\n--- Registrar Estudiante ---\n";
    string carnet = leerLinea("Carnet: ");
    if (buscarEstudiantePorCarnet(carnet) != nullptr) {
        cout << "  >> Error: Ya existe un estudiante con ese carnet.\n";
        return;
    }
    string nombre = leerLinea("Nombre completo: ");
    string carrera = leerLinea("Carrera: ");
    int semestre = leerEntero("Semestre (1-12): ");

    Estudiante est(carnet, nombre, carrera, semestre);
    estudiantes.push_back(est);
    cout << "  >> Estudiante registrado correctamente.\n";
}

void editarEstudiante() {
    cout << "\n--- Editar Estudiante ---\n";
    string carnet = leerLinea("Carnet del estudiante: ");
    Estudiante* est = buscarEstudiantePorCarnet(carnet);

    if (est == nullptr) {
        cout << "  >> Estudiante no encontrado.\n";
        return;
    }

    cout << "Datos actuales: ";
    est->mostrar();
    cout << "\nDeje en blanco para mantener el valor actual.\n";

    string nuevoNombre = leerLinea("Nombre [" + est->getNombre() + "]: ");
    if (!nuevoNombre.empty()) {
        est->setNombre(nuevoNombre);
    }

    string nuevoCarrera = leerLinea("Carrera [" + est->getCarrera() + "]: ");
    if (!nuevoCarrera.empty()) {
        est->setCarrera(nuevoCarrera);
    }

    string nuevoSem = leerLinea("Semestre [" + to_string(est->getSemestre()) + "]: ");
    if (!nuevoSem.empty()) {
        try {
            int sem = stoi(nuevoSem);
            est->setSemestre(sem);
        } catch (...) {
            cout << "  >> Valor invalido.\n";
        }
    }

    cout << "  >> Estudiante actualizado.\n";
}

void buscarEstudiante() {
    cout << "\n--- Buscar Estudiante ---\n";
    string carnet = leerLinea("Carnet: ");
    Estudiante* est = buscarEstudiantePorCarnet(carnet);

    if (est == nullptr) {
        cout << "  >> No encontrado.\n";
    } else {
        cout << "\n";
        est->mostrar();
    }
}

void listarEstudiantes() {
    cout << "\n--- Listado de Estudiantes ---\n";
    if (estudiantes.empty()) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    cout << left << setw(12) << "Carnet" << setw(25) << "Nombre"
         << setw(20) << "Carrera" << "Semestre\n";
    cout << string(70, '-') << "\n";

    for (const auto& est : estudiantes) {
        cout << left << setw(12) << est.getCarnet()
             << setw(25) << est.getNombre()
             << setw(20) << est.getCarrera()
             << est.getSemestre() << "\n";
    }
}

void eliminarEstudiante() {
    cout << "\n--- Eliminar Estudiante ---\n";
    string carnet = leerLinea("Carnet: ");
    
    for (size_t i = 0; i < estudiantes.size(); i++) {
        if (estudiantes[i].getCarnet() == carnet) {
            estudiantes.erase(estudiantes.begin() + i);
            cout << "  >> Estudiante eliminado.\n";
            return;
        }
    }
    cout << "  >> No encontrado.\n";
}

// ================================================================
// OPERACIONES CON CURSOS
// ================================================================
void registrarCurso() {
    cout << "\n--- Registrar Curso ---\n";
    string codigo = leerLinea("Codigo: ");
    if (buscarCursoPorCodigo(codigo) != nullptr) {
        cout << "  >> Error: Ya existe un curso con ese codigo.\n";
        return;
    }
    string nombre = leerLinea("Nombre: ");
    int creditos = leerEntero("Creditos (1-12): ");

    Curso cur(codigo, nombre, creditos);
    cursos.push_back(cur);
    cout << "  >> Curso registrado correctamente.\n";
}

void editarCurso() {
    cout << "\n--- Editar Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");
    Curso* cur = buscarCursoPorCodigo(codigo);

    if (cur == nullptr) {
        cout << "  >> Curso no encontrado.\n";
        return;
    }

    cout << "Datos actuales: ";
    cur->mostrar();
    cout << "\nDeje en blanco para mantener el valor actual.\n";

    string nuevoNombre = leerLinea("Nombre [" + cur->getNombre() + "]: ");
    if (!nuevoNombre.empty()) {
        cur->setNombre(nuevoNombre);
    }

    string nuevoCred = leerLinea("Creditos [" + to_string(cur->getCreditos()) + "]: ");
    if (!nuevoCred.empty()) {
        try {
            int cred = stoi(nuevoCred);
            cur->setCreditos(cred);
        } catch (...) {
            cout << "  >> Valor invalido.\n";
        }
    }

    cout << "  >> Curso actualizado.\n";
}

void buscarCurso() {
    cout << "\n--- Buscar Curso ---\n";
    string codigo = leerLinea("Codigo: ");
    Curso* cur = buscarCursoPorCodigo(codigo);

    if (cur == nullptr) {
        cout << "  >> No encontrado.\n";
    } else {
        cout << "\n";
        cur->mostrar();
    }
}

void listarCursos() {
    cout << "\n--- Listado de Cursos ---\n";
    if (cursos.empty()) {
        cout << "No hay cursos registrados.\n";
        return;
    }

    cout << left << setw(12) << "Codigo" << setw(30) << "Nombre"
         << "Creditos\n";
    cout << string(50, '-') << "\n";

    for (const auto& cur : cursos) {
        cout << left << setw(12) << cur.getCodigo()
             << setw(30) << cur.getNombre()
             << cur.getCreditos() << "\n";
    }
}

void eliminarCurso() {
    cout << "\n--- Eliminar Curso ---\n";
    string codigo = leerLinea("Codigo: ");
    
    for (size_t i = 0; i < cursos.size(); i++) {
        if (cursos[i].getCodigo() == codigo) {
            cursos.erase(cursos.begin() + i);
            cout << "  >> Curso eliminado.\n";
            return;
        }
    }
    cout << "  >> No encontrado.\n";
}

// ================================================================
// OPERACIONES CON NOTAS
// ================================================================
void registrarNota() {
    cout << "\n--- Registrar Nota ---\n";
    string carnet = leerLinea("Carnet del estudiante: ");
    string codigo = leerLinea("Codigo del curso: ");
    double calificacion = leerDouble("Calificacion (0-100): ");

    if (buscarNota(carnet, codigo) != nullptr) {
        cout << "  >> Error: Ya existe una nota para este estudiante en este curso.\n";
        return;
    }

    Nota nota(carnet, codigo, calificacion);
    notas.push_back(nota);
    cout << "  >> Nota registrada correctamente.\n";
}

void editarNota() {
    cout << "\n--- Editar Nota ---\n";
    string carnet = leerLinea("Carnet del estudiante: ");
    string codigo = leerLinea("Codigo del curso: ");
    double nuevaCal = leerDouble("Nueva calificacion: ");

    Nota* nota = buscarNota(carnet, codigo);
    if (nota == nullptr) {
        cout << "  >> Nota no encontrada.\n";
        return;
    }

    if (nota->setCalificacion(nuevaCal)) {
        cout << "  >> Nota actualizada correctamente.\n";
    }
}

void notasPorEstudiante() {
    cout << "\n--- Notas por Estudiante ---\n";
    string carnet = leerLinea("Carnet: ");

    vector<Nota> notasEst;
    for (const auto& n : notas) {
        if (n.getCarnetEstudiante() == carnet) {
            notasEst.push_back(n);
        }
    }

    if (notasEst.empty()) {
        cout << "Sin notas registradas.\n";
        return;
    }

    cout << "\n";
    for (const auto& n : notasEst) {
        n.mostrar();
    }
}

void notasPorCurso() {
    cout << "\n--- Notas por Curso ---\n";
    string codigo = leerLinea("Codigo del curso: ");

    vector<Nota> notasCur;
    for (const auto& n : notas) {
        if (n.getCodigoCurso() == codigo) {
            notasCur.push_back(n);
        }
    }

    if (notasCur.empty()) {
        cout << "Sin notas registradas.\n";
        return;
    }

    cout << "\n";
    for (const auto& n : notasCur) {
        n.mostrar();
    }
}

void eliminarNota() {
    cout << "\n--- Eliminar Nota ---\n";
    string carnet = leerLinea("Carnet del estudiante: ");
    string codigo = leerLinea("Codigo del curso: ");
    
    for (size_t i = 0; i < notas.size(); i++) {
        if (notas[i].getCarnetEstudiante() == carnet && 
            notas[i].getCodigoCurso() == codigo) {
            notas.erase(notas.begin() + i);
            cout << "  >> Nota eliminada.\n";
            return;
        }
    }
    cout << "  >> No encontrada.\n";
}

// ================================================================
// REPORTES
// ================================================================
void promedioEstudiantes() {
    cout << "\n--- REPORTE: PROMEDIO POR ESTUDIANTE ---\n";
    
    if (estudiantes.empty()) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    for (const auto& est : estudiantes) {
        vector<Nota> notasEst;
        for (const auto& n : notas) {
            if (n.getCarnetEstudiante() == est.getCarnet()) {
                notasEst.push_back(n);
            }
        }

        if (notasEst.empty()) {
            cout << est.getNombre() << " (" << est.getCarnet() << "): Sin notas\n";
            continue;
        }

        double suma = 0;
        for (const auto& nota : notasEst) {
            suma += nota.getCalificacion();
        }
        double promedio = suma / notasEst.size();
        
        cout << est.getNombre() << " (" << est.getCarnet() << "): " 
             << fixed << setprecision(2) << promedio << "\n";
    }
}

void promedioPorCurso() {
    cout << "\n--- REPORTE: PROMEDIO POR CURSO ---\n";
    
    if (cursos.empty()) {
        cout << "No hay cursos registrados.\n";
        return;
    }

    for (const auto& cur : cursos) {
        vector<Nota> notasCur;
        for (const auto& n : notas) {
            if (n.getCodigoCurso() == cur.getCodigo()) {
                notasCur.push_back(n);
            }
        }

        if (notasCur.empty()) {
            cout << cur.getNombre() << " (" << cur.getCodigo() << "): Sin notas\n";
            continue;
        }

        double suma = 0;
        for (const auto& nota : notasCur) {
            suma += nota.getCalificacion();
        }
        double promedio = suma / notasCur.size();
        
        cout << cur.getNombre() << " (" << cur.getCodigo() << "): " 
             << fixed << setprecision(2) << promedio << "\n";
    }
}

void aprobadosReprobados() {
    cout << "\n--- REPORTE: APROBADOS Y REPROBADOS ---\n";
    
    if (notas.empty()) {
        cout << "No hay notas registradas.\n";
        return;
    }

    int aprobados = 0, reprobados = 0;
    for (const auto& nota : notas) {
        if (nota.getCalificacion() >= 61.0) {
            aprobados++;
        } else {
            reprobados++;
        }
    }

    cout << "Total aprobados  : " << aprobados << "\n";
    cout << "Total reprobados : " << reprobados << "\n";
    cout << "Total notas      : " << notas.size() << "\n";
}

void mejorEstudiante() {
    cout << "\n--- REPORTE: ESTUDIANTE CON MEJOR PROMEDIO ---\n";
    
    if (estudiantes.empty()) {
        cout << "No hay estudiantes registrados.\n";
        return;
    }

    string mejorNombre = "";
    double mejorPromedio = -1.0;

    for (const auto& est : estudiantes) {
        vector<Nota> notasEst;
        for (const auto& n : notas) {
            if (n.getCarnetEstudiante() == est.getCarnet()) {
                notasEst.push_back(n);
            }
        }

        if (notasEst.empty()) continue;

        double suma = 0;
        for (const auto& nota : notasEst) {
            suma += nota.getCalificacion();
        }
        double promedio = suma / notasEst.size();

        if (promedio > mejorPromedio) {
            mejorPromedio = promedio;
            mejorNombre = est.getNombre() + " (" + est.getCarnet() + ")";
        }
    }

    if (mejorPromedio >= 0) {
        cout << "Estudiante: " << mejorNombre << "\n";
        cout << "Promedio: " << fixed << setprecision(2) << mejorPromedio << "\n";
    } else {
        cout << "No hay datos suficientes.\n";
    }
}

void expedientePorEstudiante() {
    cout << "\n--- EXPEDIENTE ACADEMICO ---\n";
    string carnet = leerLinea("Carnet del estudiante: ");
    
    Estudiante* est = buscarEstudiantePorCarnet(carnet);
    if (est == nullptr) {
        cout << "Estudiante no encontrado.\n";
        return;
    }

    cout << "\nCarnet: " << est->getCarnet() << "\n";
    cout << "Nombre: " << est->getNombre() << "\n";
    cout << "Carrera: " << est->getCarrera() << "\n";
    cout << "Semestre: " << est->getSemestre() << "\n";
    cout << "\nHistorial de Cursos:\n";
    cout << string(70, '-') << "\n";

    vector<Nota> notasEst;
    for (const auto& n : notas) {
        if (n.getCarnetEstudiante() == carnet) {
            notasEst.push_back(n);
        }
    }

    if (notasEst.empty()) {
        cout << "Sin notas registradas.\n";
        return;
    }

    double sumaPromedios = 0;
    for (const auto& nota : notasEst) {
        Curso* cur = buscarCursoPorCodigo(nota.getCodigoCurso());
        string nombreCurso = (cur != nullptr) ? cur->getNombre() : "(no encontrado)";
        
        cout << nombreCurso << " [" << nota.getCodigoCurso() << "]: "
             << fixed << setprecision(2) << nota.getCalificacion() 
             << " - " << nota.getEstado() << "\n";
        
        sumaPromedios += nota.getCalificacion();
    }

    cout << string(70, '-') << "\n";
    double promedio = sumaPromedios / notasEst.size();
    cout << "Promedio General: " << fixed << setprecision(2) << promedio << "\n";
}

// ================================================================
// PERSISTENCIA (Guardar y Cargar)
// ================================================================
void guardarDatos() {
    // Guardar estudiantes
    ofstream file1("datos_estudiantes.txt");
    if (file1.is_open()) {
        file1 << estudiantes.size() << "\n";
        for (const auto& est : estudiantes) {
            file1 << est.getCarnet() << "\n";
            file1 << est.getNombre() << "\n";
            file1 << est.getCarrera() << "\n";
            file1 << est.getSemestre() << "\n";
        }
        file1.close();
    }

    // Guardar cursos
    ofstream file2("datos_cursos.txt");
    if (file2.is_open()) {
        file2 << cursos.size() << "\n";
        for (const auto& cur : cursos) {
            file2 << cur.getCodigo() << "\n";
            file2 << cur.getNombre() << "\n";
            file2 << cur.getCreditos() << "\n";
        }
        file2.close();
    }

    // Guardar notas
    ofstream file3("datos_notas.txt");
    if (file3.is_open()) {
        file3 << notas.size() << "\n";
        for (const auto& nota : notas) {
            file3 << nota.getCarnetEstudiante() << "\n";
            file3 << nota.getCodigoCurso() << "\n";
            file3 << nota.getCalificacion() << "\n";
        }
        file3.close();
    }

    cout << "  >> Datos guardados correctamente.\n";
}

void cargarDatos() {
    // Cargar estudiantes
    ifstream file1("datos_estudiantes.txt");
    if (file1.is_open()) {
        estudiantes.clear();
        int cantidad;
        file1 >> cantidad;
        file1.ignore();
        
        for (int i = 0; i < cantidad; i++) {
            string carnet, nombre, carrera;
            int semestre;
            
            getline(file1, carnet);
            getline(file1, nombre);
            getline(file1, carrera);
            file1 >> semestre;
            file1.ignore();
            
            Estudiante est(carnet, nombre, carrera, semestre);
            estudiantes.push_back(est);
        }
        file1.close();
    }

    // Cargar cursos
    ifstream file2("datos_cursos.txt");
    if (file2.is_open()) {
        cursos.clear();
        int cantidad;
        file2 >> cantidad;
        file2.ignore();
        
        for (int i = 0; i < cantidad; i++) {
            string codigo, nombre;
            int creditos;
            
            getline(file2, codigo);
            getline(file2, nombre);
            file2 >> creditos;
            file2.ignore();
            
            Curso cur(codigo, nombre, creditos);
            cursos.push_back(cur);
        }
        file2.close();
    }

    // Cargar notas
    ifstream file3("datos_notas.txt");
    if (file3.is_open()) {
        notas.clear();
        int cantidad;
        file3 >> cantidad;
        file3.ignore();
        
        for (int i = 0; i < cantidad; i++) {
            string carnet, codigo;
            double calificacion;
            
            getline(file3, carnet);
            getline(file3, codigo);
            file3 >> calificacion;
            file3.ignore();
            
            Nota nota(carnet, codigo, calificacion);
            notas.push_back(nota);
        }
        file3.close();
    }

    cout << "  >> Datos cargados correctamente.\n";
}

// ================================================================
// MENUS
// ================================================================
void menuEstudiantes() {
    int opcion;
    do {
        cout << "\n===== MENU ESTUDIANTES =====\n"
             << "1. Registrar estudiante\n"
             << "2. Editar estudiante\n"
             << "3. Buscar estudiante\n"
             << "4. Listar todos\n"
             << "5. Eliminar estudiante\n"
             << "0. Volver\n";
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: registrarEstudiante(); break;
            case 2: editarEstudiante(); break;
            case 3: buscarEstudiante(); break;
            case 4: listarEstudiantes(); break;
            case 5: eliminarEstudiante(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void menuCursos() {
    int opcion;
    do {
        cout << "\n===== MENU CURSOS =====\n"
             << "1. Registrar curso\n"
             << "2. Editar curso\n"
             << "3. Buscar curso\n"
             << "4. Listar todos\n"
             << "5. Eliminar curso\n"
             << "0. Volver\n";
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: registrarCurso(); break;
            case 2: editarCurso(); break;
            case 3: buscarCurso(); break;
            case 4: listarCursos(); break;
            case 5: eliminarCurso(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void menuNotas() {
    int opcion;
    do {
        cout << "\n===== MENU NOTAS =====\n"
             << "1. Registrar nota\n"
             << "2. Editar nota\n"
             << "3. Notas por estudiante\n"
             << "4. Notas por curso\n"
             << "5. Eliminar nota\n"
             << "0. Volver\n";
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: registrarNota(); break;
            case 2: editarNota(); break;
            case 3: notasPorEstudiante(); break;
            case 4: notasPorCurso(); break;
            case 5: eliminarNota(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

void menuReportes() {
    int opcion;
    do {
        cout << "\n===== MENU REPORTES =====\n"
             << "1. Promedio por estudiante\n"
             << "2. Promedio por curso\n"
             << "3. Aprobados y reprobados\n"
             << "4. Mejor estudiante\n"
             << "5. Expediente de estudiante\n"
             << "0. Volver\n";
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: promedioEstudiantes(); break;
            case 2: promedioPorCurso(); break;
            case 3: aprobadosReprobados(); break;
            case 4: mejorEstudiante(); break;
            case 5: expedientePorEstudiante(); break;
            case 0: break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);
}

// ================================================================
// MAIN
// ================================================================
int main() {
    int opcion;

    cout << "Cargando datos...\n";
    cargarDatos();

    do {
        cout << "\n=========================================\n"
             << "     SISTEMA ACADEMICO - MENU PRINCIPAL\n"
             << "=========================================\n"
             << "1. Modulo Estudiantes\n"
             << "2. Modulo Cursos\n"
             << "3. Modulo Notas\n"
             << "4. Reportes y Estadisticas\n"
             << "5. Guardar datos\n"
             << "0. Salir\n";
        
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: menuEstudiantes(); break;
            case 2: menuCursos(); break;
            case 3: menuNotas(); break;
            case 4: menuReportes(); break;
            case 5: guardarDatos(); break;
            case 0:
                cout << "Guardando datos antes de salir...\n";
                guardarDatos();
                cout << "Hasta pronto!\n";
                break;
            default: cout << "  >> Opcion invalida.\n";
        }
    } while (opcion != 0);

    return 0;
}
