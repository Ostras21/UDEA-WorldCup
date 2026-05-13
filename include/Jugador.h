#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <iostream>
#include "EstadisticaJugador.h"


class Jugador {
private:
    std::string nombre;
    std::string apellido;
    int numeroCamiseta;
    EstadisticaJugador estadistica;

public:
    Jugador();
    Jugador(const std::string& nombre, const std::string& apellido,
            int numeroCamiseta);
    Jugador(const std::string& nombre, const std::string& apellido,
            int numeroCamiseta, const EstadisticaJugador& estadistica);

    //  Getters
    std::string getNombre()        const;
    std::string getApellido()      const;
    int         getNumeroCamiseta() const;

    // Acceso a la estadistica (referencia para permitir modificacion).
    EstadisticaJugador& getEstadistica();
    const EstadisticaJugador& getEstadistica() const;

    //  Setters
    void setNombre(const std::string& v);
    void setApellido(const std::string& v);
    void setNumeroCamiseta(int v);
    void setEstadistica(const EstadisticaJugador& e);

    // Actualiza la estadistica historica con los datos de un partido.
    void actualizar(int golesPartido, int minutosPartido,
                    int amarillasPartido, int rojasPartido,
                    int faltasPartido, int asistenciasPartido);

    friend std::ostream& operator<<(std::ostream& salida, const Jugador& j);
};

#endif // JUGADOR_H
