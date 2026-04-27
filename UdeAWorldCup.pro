#-------------------------------------------------
# Proyecto: UdeAWorldCup
# Desafio II - Informatica II - Semestre 2026-1
# Simulacion de la Copa Mundial de Futbol FIFA 2026
#-------------------------------------------------

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

# TARGET y DESTDIR explicitos:
#   - TARGET fija el nombre del ejecutable (sin TARGET, qmake lo deriva
#     del .pro y a veces Qt Creator se desincroniza -> "No executable
#     specified" al intentar correr).
#   - DESTDIR coloca el .exe en la raiz del proyecto (la misma carpeta
#     donde vive el directorio data/), de modo que la ruta relativa
#     "data/selecciones_clasificadas_mundial.csv" funciona tanto al
#     correr desde terminal como desde Qt (siempre que la Working
#     directory de Qt apunte tambien a la raiz del proyecto).
TARGET = udeaworldcup
DESTDIR = $$PWD

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/Fecha.cpp \
    src/Confederacion.cpp \
    src/EstadisticaJugador.cpp \
    src/EstadisticaEquipo.cpp \
    src/Jugador.cpp \
    src/Equipo.cpp \
    src/MedidorRecursos.cpp \
    src/RegistroJugadorPartido.cpp \
    src/EstadisticaPartido.cpp \
    src/Bombo.cpp \
    src/Partido.cpp \
    src/Grupo.cpp \
    src/Torneo.cpp

HEADERS += \
    include/Lista.h \
    include/Fecha.h \
    include/Confederacion.h \
    include/EstadisticaJugador.h \
    include/EstadisticaEquipo.h \
    include/Jugador.h \
    include/Equipo.h \
    include/MedidorRecursos.h \
    include/RegistroJugadorPartido.h \
    include/EstadisticaPartido.h \
    include/Bombo.h \
    include/Partido.h \
    include/Grupo.h \
    include/Torneo.h

QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
