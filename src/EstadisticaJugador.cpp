#include "EstadisticaJugador.h"

EstadisticaJugador::EstadisticaJugador()
    : partidosJugados(0), goles(0), minutos(0), asistencias(0),
      amarillas(0), rojas(0), faltas(0) {}

EstadisticaJugador::EstadisticaJugador(int partidosJugados, int goles, int minutos,
                                       int asistencias, int amarillas, int rojas, int faltas)
    : partidosJugados(partidosJugados), goles(goles), minutos(minutos),
      asistencias(asistencias), amarillas(amarillas), rojas(rojas), faltas(faltas) {}

// ---------- Getters ----------
int EstadisticaJugador::getPartidosJugados() const { return partidosJugados; }
int EstadisticaJugador::getGoles()           const { return goles; }
int EstadisticaJugador::getMinutos()         const { return minutos; }
int EstadisticaJugador::getAsistencias()     const { return asistencias; }
int EstadisticaJugador::getAmarillas()       const { return amarillas; }
int EstadisticaJugador::getRojas()           const { return rojas; }
int EstadisticaJugador::getFaltas()          const { return faltas; }

// ---------- Setters ----------
void EstadisticaJugador::setGoles(int goles)            { this->goles = goles; }
void EstadisticaJugador::setPartidosJugados(int p)      { partidosJugados = p; }

// ---------- Acumulacion manual (antes de tener RegistroJugadorPartido) ----------
void EstadisticaJugador::acumularPartido(int golesPartido, int minutosPartido,
                                         int amarillasPartido, int rojasPartido,
                                         int faltasPartido, int asistenciasPartido) {
    ++partidosJugados;
    goles       += golesPartido;
    minutos     += minutosPartido;
    amarillas   += amarillasPartido;
    rojas       += rojasPartido;
    faltas      += faltasPartido;
    asistencias += asistenciasPartido;
}

// ---------- operator+= ----------
EstadisticaJugador& EstadisticaJugador::operator+=(const EstadisticaJugador& otra) {
    partidosJugados += otra.partidosJugados;
    goles           += otra.goles;
    minutos         += otra.minutos;
    asistencias     += otra.asistencias;
    amarillas       += otra.amarillas;
    rojas           += otra.rojas;
    faltas          += otra.faltas;
    return *this;
}

// ---------- Impresion ----------
std::ostream& operator<<(std::ostream& salida, const EstadisticaJugador& e) {
    salida << "PJ=" << e.partidosJugados
           << " G="  << e.goles
           << " Min=" << e.minutos
           << " Asis=" << e.asistencias
           << " TA=" << e.amarillas
           << " TR=" << e.rojas
           << " Fal=" << e.faltas;
    return salida;
}
