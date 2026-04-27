#include "RegistroJugadorPartido.h"

RegistroJugadorPartido::RegistroJugadorPartido()
    : jugador(nullptr), golesPartido(0), minutosJugados(0),
      amarillas(0), rojas(0), faltas(0), asistencias(0) {}

RegistroJugadorPartido::RegistroJugadorPartido(Jugador* jugador)
    : jugador(jugador), golesPartido(0), minutosJugados(0),
      amarillas(0), rojas(0), faltas(0), asistencias(0) {}

RegistroJugadorPartido::RegistroJugadorPartido(Jugador* jugador, int goles, int minutos,
                                               int amarillas, int rojas, int faltas,
                                               int asistencias)
    : jugador(jugador), golesPartido(goles), minutosJugados(minutos),
      amarillas(amarillas), rojas(rojas), faltas(faltas), asistencias(asistencias) {}

// ---------- Getters ----------
Jugador* RegistroJugadorPartido::getJugador()    const { return jugador; }
int RegistroJugadorPartido::getGolesPartido()    const { return golesPartido; }
int RegistroJugadorPartido::getMinutosJugados()  const { return minutosJugados; }
int RegistroJugadorPartido::getAmarillas()       const { return amarillas; }
int RegistroJugadorPartido::getRojas()           const { return rojas; }
int RegistroJugadorPartido::getFaltas()          const { return faltas; }
int RegistroJugadorPartido::getAsistencias()     const { return asistencias; }

// ---------- Setters ----------
void RegistroJugadorPartido::setJugador(Jugador* j)    { jugador = j; }
void RegistroJugadorPartido::setGolesPartido(int v)    { golesPartido = v; }
void RegistroJugadorPartido::setMinutosJugados(int v)  { minutosJugados = v; }
void RegistroJugadorPartido::setAmarillas(int v)       { amarillas = v; }
void RegistroJugadorPartido::setRojas(int v)           { rojas = v; }
void RegistroJugadorPartido::setFaltas(int v)          { faltas = v; }
void RegistroJugadorPartido::setAsistencias(int v)     { asistencias = v; }

// ---------- Acumulacion ----------
void RegistroJugadorPartido::agregarGol()        { ++golesPartido; }
void RegistroJugadorPartido::agregarAmarilla()   { ++amarillas; }
void RegistroJugadorPartido::agregarRoja()       { ++rojas; }
void RegistroJugadorPartido::agregarFalta()      { ++faltas; }
void RegistroJugadorPartido::agregarAsistencia() { ++asistencias; }

std::ostream& operator<<(std::ostream& salida, const RegistroJugadorPartido& r) {
    salida << "Reg{";
    if (r.jugador != nullptr) {
        salida << "#" << r.jugador->getNumeroCamiseta()
               << " " << r.jugador->getApellido();
    } else {
        salida << "(sin jugador)";
    }
    salida << " G=" << r.golesPartido
           << " min=" << r.minutosJugados
           << " A=" << r.amarillas
           << " R=" << r.rojas
           << " F=" << r.faltas
           << " As=" << r.asistencias
           << "}";
    return salida;
}
