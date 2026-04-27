#include "EstadisticaPartido.h"

EstadisticaPartido::EstadisticaPartido()
    : registros(), golesFavor(0), golesContra(0), posesion(50.0) {}

EstadisticaPartido::EstadisticaPartido(int golesFavor, int golesContra, double posesion)
    : registros(), golesFavor(golesFavor), golesContra(golesContra),
      posesion(posesion) {}

// ---------- Getters ----------
Lista<RegistroJugadorPartido>&       EstadisticaPartido::getRegistros()       { return registros; }
const Lista<RegistroJugadorPartido>& EstadisticaPartido::getRegistros() const { return registros; }
int    EstadisticaPartido::getGolesFavor()  const { return golesFavor; }
int    EstadisticaPartido::getGolesContra() const { return golesContra; }
double EstadisticaPartido::getPosesion()    const { return posesion; }

// ---------- Setters ----------
void EstadisticaPartido::setGolesFavor(int v)  { golesFavor = v; }
void EstadisticaPartido::setGolesContra(int v) { golesContra = v; }
void EstadisticaPartido::setPosesion(double v) { posesion = v; }

// ---------- Operaciones ----------
void EstadisticaPartido::agregarRegistro(const RegistroJugadorPartido& r) {
    registros.agregar(r);
}

int EstadisticaPartido::cantidadRegistros() const {
    return registros.getTamano();
}

std::ostream& operator<<(std::ostream& salida, const EstadisticaPartido& e) {
    salida << "EstPartido{GF=" << e.golesFavor
           << " GC=" << e.golesContra
           << " pos=" << e.posesion << "%"
           << " convocados=" << e.registros.getTamano()
           << "}";
    return salida;
}
