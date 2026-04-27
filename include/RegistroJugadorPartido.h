#ifndef REGISTROJUGADORPARTIDO_H
#define REGISTROJUGADORPARTIDO_H

#include <iostream>
#include "Jugador.h"

/*
 * Clase RegistroJugadorPartido
 * --------------------------------------------------------------
 * Representa la actuacion individual de un jugador en un partido
 * concreto del torneo. Es una "fila" del registro de eventos:
 *   - puntero al jugador (no se posee, vive en su Equipo)
 *   - goles anotados en ESTE partido
 *   - minutos jugados, amarillas, rojas, faltas, asistencias
 *
 * Decisiones de diseño:
 *  - El jugador se referencia por puntero (agregacion). El ciclo
 *    de vida del jugador esta en su Equipo, no aqui.
 *  - No maneja memoria dinamica propia -> no requiere Regla de
 *    los Tres (las versiones sinteticas funcionan).
 *  - Los metodos agregarGol/agregarAmarilla/etc. permiten que el
 *    simulador de partido vaya acumulando eventos uno a uno
 *    durante la simulacion (Bloque 4).
 *  - operator<< como funcion amiga (requisito UML).
 * --------------------------------------------------------------
 */
class RegistroJugadorPartido {
private:
    Jugador* jugador;       // observador, no se destruye aqui
    int golesPartido;
    int minutosJugados;
    int amarillas;
    int rojas;
    int faltas;
    int asistencias;

public:
    RegistroJugadorPartido();
    RegistroJugadorPartido(Jugador* jugador);
    RegistroJugadorPartido(Jugador* jugador, int goles, int minutos,
                           int amarillas, int rojas, int faltas, int asistencias);

    // ---------- Getters ----------
    Jugador* getJugador()       const;
    int getGolesPartido()       const;
    int getMinutosJugados()     const;
    int getAmarillas()          const;
    int getRojas()              const;
    int getFaltas()             const;
    int getAsistencias()        const;

    // ---------- Setters ----------
    void setJugador(Jugador* j);
    void setGolesPartido(int v);
    void setMinutosJugados(int v);
    void setAmarillas(int v);
    void setRojas(int v);
    void setFaltas(int v);
    void setAsistencias(int v);

    // ---------- Acumulacion de eventos individuales ----------
    void agregarGol();
    void agregarAmarilla();
    void agregarRoja();
    void agregarFalta();
    void agregarAsistencia();

    friend std::ostream& operator<<(std::ostream& salida, const RegistroJugadorPartido& r);
};

#endif // REGISTROJUGADORPARTIDO_H
