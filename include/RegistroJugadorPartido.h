#ifndef REGISTROJUGADORPARTIDO_H
#define REGISTROJUGADORPARTIDO_H

#include <iostream>
#include "Jugador.h"

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

    //  Getters
    Jugador* getJugador()       const;
    int getGolesPartido()       const;
    int getMinutosJugados()     const;
    int getAmarillas()          const;
    int getRojas()              const;
    int getFaltas()             const;
    int getAsistencias()        const;

    //  Setters
    void setJugador(Jugador* j);
    void setGolesPartido(int v);
    void setMinutosJugados(int v);
    void setAmarillas(int v);
    void setRojas(int v);
    void setFaltas(int v);
    void setAsistencias(int v);

    //  Acumulacion de eventos individuales
    void agregarGol();
    void agregarAmarilla();
    void agregarRoja();
    void agregarFalta();
    void agregarAsistencia();

    friend std::ostream& operator<<(std::ostream& salida, const RegistroJugadorPartido& r);
};

#endif // REGISTROJUGADORPARTIDO_H
