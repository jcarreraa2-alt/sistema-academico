# ===================================================
# Makefile - Sistema Academico
# ===================================================

CXX     = g++
CXXFLAGS = -std=c++11 -O2 -Wall
TARGET  = sistema_academico

SRCS    = src/main.cpp \
          src/utilidades.cpp \
          src/estudiantes.cpp \
          src/cursos_notas.cpp \
          src/procesamiento.cpp \
          src/reportes.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
