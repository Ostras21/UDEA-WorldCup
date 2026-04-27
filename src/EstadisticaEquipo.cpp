#include "EstadisticaEquipo.h"

EstadisticaEquipo::EstadisticaEquipo()
    : golesFavor(0), golesContra(0), ganados(0), empatados(0),
      perdidos(0), amarillas(0), rojas(0), faltas(0) {}

EstadisticaEquipo::EstadisticaEquipo(int gf, int gc, int g, int e, int p,
                                     int am, int ro, int fa)
    : golesFavor(gf), golesContra(gc), ganados(g), empatados(e),
      perdidos(p), amarillas(am), rojas(ro), faltas(fa) {}

// ---------- Getters ----------
int EstadisticaEquipo::getGolesFavor()  const { return golesFavor; }
int EstadisticaEquipo::getGolesContra() const { return golesContra; }
int EstadisticaEquipo::getGanados()     const { return ganados; }
int EstadisticaEquipo::getEmpatados()   const { return empatados; }
int EstadisticaEquipo::getPerdidos()    const { return perdidos; }
int EstadisticaEquipo::getAmarillas()   const { return amarillas; }
int EstadisticaEquipo::getRojas()       const { return rojas; }
int EstadisticaEquipo::getFaltas()      const { return faltas; }

int EstadisticaEquipo::getPartidosJugados() const {
    return ganados + empatados + perdidos;
}

int EstadisticaEquipo::getPuntos() const {
    return ganados * 3 + empatados;
}

int EstadisticaEquipo::getDiferenciaGoles() const {
    return golesFavor - golesContra;
}

double EstadisticaEquipo::promedioGolesFavor() const {
    int pj = getPartidosJugados();
    return (pj > 0) ? static_cast<double>(golesFavor) / pj : 1.0;
}

double EstadisticaEquipo::promedioGolesContra() const {
    int pj = getPartidosJugados();
    return (pj > 0) ? static_cast<double>(golesContra) / pj : 1.0;
}

// ---------- Setters ----------
void EstadisticaEquipo::setGolesFavor(int v)  { golesFavor = v; }
void EstadisticaEquipo::setGolesContra(int v) { golesContra = v; }
void EstadisticaEquipo::setGanados(int v)     { ganados = v; }
void EstadisticaEquipo::setEmpatados(int v)   { empatados = v; }
void EstadisticaEquipo::setPerdidos(int v)    { perdidos = v; }

// ---------- Acumulacion ----------
void EstadisticaEquipo::acumularPartido(int golesFavorPart, int golesContraPart,
                                        int amarillasPart, int rojasPart,
                                        int faltasPart, char resultado) {
    golesFavor  += golesFavorPart;
    golesContra += golesContraPart;
    amarillas   += amarillasPart;
    rojas       += rojasPart;
    faltas      += faltasPart;
    if (resultado == 'V')      ++ganados;
    else if (resultado == 'E') ++empatados;
    else                       ++perdidos; // 'D' o cualquier otro
}

EstadisticaEquipo& EstadisticaEquipo::operator+=(const EstadisticaEquipo& otra) {
    golesFavor  += otra.golesFavor;
    golesContra += otra.golesContra;
    ganados     += otra.ganados;
    empatados   += otra.empatados;
    perdidos    += otra.perdidos;
    amarillas   += otra.amarillas;
    rojas       += otra.rojas;
    faltas      += otra.faltas;
    return *this;
}

std::ostream& operator<<(std::ostream& salida, const EstadisticaEquipo& e) {
    salida << "PJ=" << e.getPartidosJugados()
           << " G=" << e.ganados
           << " E=" << e.empatados
           << " P=" << e.perdidos
           << " GF=" << e.golesFavor
           << " GC=" << e.golesContra
           << " DG=" << e.getDiferenciaGoles()
           << " Pts=" << e.getPuntos();
    return salida;
}
