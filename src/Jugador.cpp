#include "Jugador.h"

Jugador::Jugador()
    : nombre(""), apellido(""), numeroCamiseta(0), estadistica() {}

Jugador::Jugador(const std::string& nombre, const std::string& apellido,
                 int numeroCamiseta)
    : nombre(nombre), apellido(apellido),
      numeroCamiseta(numeroCamiseta), estadistica() {}

Jugador::Jugador(const std::string& nombre, const std::string& apellido,
                 int numeroCamiseta, const EstadisticaJugador& estadistica)
    : nombre(nombre), apellido(apellido),
      numeroCamiseta(numeroCamiseta), estadistica(estadistica) {}

//  Getters
std::string Jugador::getNombre()         const { return nombre; }
std::string Jugador::getApellido()       const { return apellido; }
int         Jugador::getNumeroCamiseta() const { return numeroCamiseta; }

EstadisticaJugador& Jugador::getEstadistica() { return estadistica; }
const EstadisticaJugador& Jugador::getEstadistica() const { return estadistica; }

//  setters
void Jugador::setNombre(const std::string& v)         { nombre = v; }
void Jugador::setApellido(const std::string& v)       { apellido = v; }
void Jugador::setNumeroCamiseta(int v)                { numeroCamiseta = v; }
void Jugador::setEstadistica(const EstadisticaJugador& e) { estadistica = e; }

//  actualizacion
void Jugador::actualizar(int golesPartido, int minutosPartido,
                         int amarillasPartido, int rojasPartido,
                         int faltasPartido, int asistenciasPartido) {
    estadistica.acumularPartido(golesPartido, minutosPartido,
                                amarillasPartido, rojasPartido,
                                faltasPartido, asistenciasPartido);
}

std::ostream& operator<<(std::ostream& salida, const Jugador& j) {
    salida << "[" << j.numeroCamiseta << "] "
           << j.nombre << " " << j.apellido
           << " { " << j.estadistica << " }";
    return salida;
}
