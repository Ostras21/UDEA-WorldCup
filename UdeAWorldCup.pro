#-------------------------------------------------
# Proyecto: UdeAWorldCup
# Desafio II - Informatica II - Semestre 2026-1
# Simulacion de la Copa Mundial de Futbol FIFA 2026
#-------------------------------------------------

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

# Carpetas de busqueda de cabeceras
INCLUDEPATH += include

# Archivos fuente
SOURCES += \
    src/main.cpp \
    src/Fecha.cpp \
    src/Confederacion.cpp \
    src/EstadisticaJugador.cpp

# Archivos de cabecera
HEADERS += \
    include/Lista.h \
    include/Fecha.h \
    include/Confederacion.h \
    include/EstadisticaJugador.h

# Flags de compilacion estrictos (buena practica)
QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
