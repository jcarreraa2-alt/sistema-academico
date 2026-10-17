# Sistema Academico

Proyecto de consola en C++11 organizado en módulos separados.

## Estructura del proyecto

```
sistema_academico/
├── Makefile
├── README.md
├── include/                  # Cabeceras (.h)
│   ├── datos.h               # Estructuras, constantes y extern de vectores globales
│   ├── utilidades.h          # Entrada de datos y búsquedas básicas
│   ├── estudiantes.h         # Módulo de Estudiantes
│   ├── cursos_notas.h        # Módulo de Cursos y Notas
│   ├── procesamiento.h       # Módulo de cálculo recursivo
│   └── reportes.h            # Módulo de Reportes y Persistencia
└── src/                      # Implementaciones (.cpp)
    ├── main.cpp              # Punto de entrada y menú principal
    ├── utilidades.cpp
    ├── estudiantes.cpp
    ├── cursos_notas.cpp
    ├── procesamiento.cpp
    └── reportes.cpp
```

## Compilar

### Con Make (recomendado)
```bash
make
```

### Manualmente
```bash
g++ -std=c++11 -O2 -o sistema_academico \
    src/main.cpp src/utilidades.cpp src/estudiantes.cpp \
    src/cursos_notas.cpp src/procesamiento.cpp src/reportes.cpp
```

## Ejecutar

```bash
./sistema_academico
```

## Módulos

| Módulo | Archivos | Descripción |
|--------|----------|-------------|
| Datos | `include/datos.h` | Estructuras `Estudiante`, `Curso`, `Nota` y constantes |
| Utilidades | `utilidades.h / .cpp` | Entrada validada de datos y búsquedas |
| Estudiantes | `estudiantes.h / .cpp` | Registrar, editar, buscar y listar estudiantes |
| Cursos y Notas | `cursos_notas.h / .cpp` | CRUD de cursos y registro de notas |
| Procesamiento | `procesamiento.h / .cpp` | Promedios y conteos con funciones recursivas |
| Reportes | `reportes.h / .cpp` | Reportes estadísticos y persistencia en archivo |
