#include "Grupo.h"
#include "Partido.h"   // requerido aqui para usar metodos de Partido
#include <stdexcept>
#include <random>

// RNG estatico para el sorteo aleatorio que rompe igualdades en la
// tabla cuando puntos, diferencia de goles y goles a favor coinciden.
// Mismo patron usado en Bombo y Partido.
static std::mt19937& rngGrupo() {
    static std::mt19937 g(std::random_device{}());
    return g;
}

Grupo::Grupo()
    : letra(' '), equipos(), partidos(), tablaCalculada(false) {
    for (int i = 0; i < 4; ++i) {
        puntos[i] = 0; gfTabla[i] = 0; gcTabla[i] = 0;
        dgTabla[i] = 0; pgArr[i] = 0; peArr[i] = 0; ppArr[i] = 0;
        posicion[i] = i;
    }
}

Grupo::Grupo(char letra)
    : letra(letra), equipos(), partidos(), tablaCalculada(false) {
    for (int i = 0; i < 4; ++i) {
        puntos[i] = 0; gfTabla[i] = 0; gcTabla[i] = 0;
        dgTabla[i] = 0; pgArr[i] = 0; peArr[i] = 0; ppArr[i] = 0;
        posicion[i] = i;
    }
}

// ---------- Getters ----------
char Grupo::getLetra() const                          { return letra; }
Lista<Equipo*>&       Grupo::getEquipos()             { return equipos; }
const Lista<Equipo*>& Grupo::getEquipos()       const { return equipos; }
Lista<Partido*>&      Grupo::getPartidos()            { return partidos; }
const Lista<Partido*>& Grupo::getPartidos()     const { return partidos; }
int Grupo::cantidadEquipos()  const                   { return equipos.getTamano(); }
int Grupo::cantidadPartidos() const                   { return partidos.getTamano(); }

// ---------- Setters ----------
void Grupo::setLetra(char l) { letra = l; }

// ---------- Composicion ----------
void Grupo::agregarEquipo(Equipo* e) {
    if (e == nullptr) return;
    if (equipos.getTamano() >= 4) return;   // grupo lleno
    equipos.agregar(e);
    tablaCalculada = false;
}

void Grupo::agregarPartido(Partido* p) {
    if (p == nullptr) return;
    if (partidos.getTamano() >= 6) return;  // round-robin completo
    partidos.agregar(p);
    tablaCalculada = false;
}

// ---------- Tabla ----------
//
// Localiza el indice (0..3) de un equipo dentro de equipos[]
// comparando por puntero. Devuelve -1 si no esta.
static int indiceEquipo(const Lista<Equipo*>& eqs, const Equipo* e) {
    for (int i = 0; i < eqs.getTamano(); ++i) {
        if (eqs[i] == e) return i;
    }
    return -1;
}

void Grupo::calcularTabla() {
    // Reset
    for (int i = 0; i < 4; ++i) {
        puntos[i] = 0; gfTabla[i] = 0; gcTabla[i] = 0;
        dgTabla[i] = 0; pgArr[i] = 0; peArr[i] = 0; ppArr[i] = 0;
        posicion[i] = i;
    }

    // Acumular partidos simulados
    for (int p = 0; p < partidos.getTamano(); ++p) {
        const Partido* part = partidos[p];
        if (part == nullptr || !part->fueSimulado()) continue;

        int i1 = indiceEquipo(equipos, part->getEquipo1());
        int i2 = indiceEquipo(equipos, part->getEquipo2());
        if (i1 < 0 || i2 < 0) continue;

        int g1 = part->getEstadistica1().getGolesFavor();
        int g2 = part->getEstadistica2().getGolesFavor();

        gfTabla[i1] += g1; gcTabla[i1] += g2;
        gfTabla[i2] += g2; gcTabla[i2] += g1;

        if (g1 > g2) {
            puntos[i1] += 3; ++pgArr[i1]; ++ppArr[i2];
        } else if (g2 > g1) {
            puntos[i2] += 3; ++pgArr[i2]; ++ppArr[i1];
        } else {
            puntos[i1] += 1; puntos[i2] += 1;
            ++peArr[i1]; ++peArr[i2];
        }
    }

    for (int i = 0; i < 4; ++i) {
        dgTabla[i] = gfTabla[i] - gcTabla[i];
    }

    // Tiebreak aleatorio: una etiqueta por equipo. Si puntos, DG y GF
    // coinciden entre dos, decide quien va arriba el menor de estos
    // numeros (sorteo). Cada llamada a calcularTabla produce nuevas
    // etiquetas: el sorteo se "vuelve a hacer" cada vez.
    int tieBreak[4];
    std::uniform_int_distribution<int> dist(0, 1000000);
    for (int i = 0; i < 4; ++i) tieBreak[i] = dist(rngGrupo());

    // Bubble sort por: puntos DESC, DG DESC, GF DESC, tieBreak ASC.
    int n = equipos.getTamano();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            int a = posicion[j], b = posicion[j + 1];
            bool intercambiar = false;
            if (puntos[a] < puntos[b]) intercambiar = true;
            else if (puntos[a] == puntos[b]) {
                if (dgTabla[a] < dgTabla[b]) intercambiar = true;
                else if (dgTabla[a] == dgTabla[b]) {
                    if (gfTabla[a] < gfTabla[b]) intercambiar = true;
                    else if (gfTabla[a] == gfTabla[b] && tieBreak[a] > tieBreak[b]) {
                        intercambiar = true;
                    }
                }
            }
            if (intercambiar) {
                int tmp = posicion[j];
                posicion[j] = posicion[j + 1];
                posicion[j + 1] = tmp;
            }
        }
    }

    tablaCalculada = true;
}

Lista<Equipo*> Grupo::getClasificados() const {
    Lista<Equipo*> top2;
    int n = equipos.getTamano();
    for (int i = 0; i < n && i < 2; ++i) {
        top2.agregar(equipos[posicion[i]]);
    }
    return top2;
}

Equipo* Grupo::getTercero() const {
    if (equipos.getTamano() < 3) return nullptr;
    return equipos[posicion[2]];
}

Equipo* Grupo::getEquipoEnPosicion(int pos) const {
    if (pos < 0 || pos >= equipos.getTamano()) {
        throw std::out_of_range("Grupo::getEquipoEnPosicion: pos fuera de rango");
    }
    return equipos[posicion[pos]];
}

int Grupo::getPuntosEnPosicion(int pos) const {
    if (pos < 0 || pos >= equipos.getTamano()) {
        throw std::out_of_range("Grupo::getPuntosEnPosicion: pos fuera de rango");
    }
    return puntos[posicion[pos]];
}

int Grupo::getDGEnPosicion(int pos) const {
    if (pos < 0 || pos >= equipos.getTamano()) {
        throw std::out_of_range("Grupo::getDGEnPosicion: pos fuera de rango");
    }
    return dgTabla[posicion[pos]];
}

int Grupo::getGFEnPosicion(int pos) const {
    if (pos < 0 || pos >= equipos.getTamano()) {
        throw std::out_of_range("Grupo::getGFEnPosicion: pos fuera de rango");
    }
    return gfTabla[posicion[pos]];
}

std::ostream& operator<<(std::ostream& salida, const Grupo& g) {
    salida << "GRUPO " << g.letra
           << "  (" << g.equipos.getTamano() << " eq, "
           << g.partidos.getTamano() << " partidos)";
    if (!g.tablaCalculada) {
        salida << "  [tabla sin calcular]";
        return salida;
    }
    salida << "\n  Pos  Equipo                       PJ  PG  PE  PP  GF  GC   DG  Pts";
    for (int i = 0; i < g.equipos.getTamano(); ++i) {
        int idx = g.posicion[i];
        const Equipo* eq = g.equipos[idx];
        std::string etiqueta = "(sin equipo)";
        if (eq != nullptr) {
            etiqueta = eq->getCodigo() + " " + eq->getPais();
        }
        // Padding manual para columnas alineadas.
        if (etiqueta.size() > 28) etiqueta = etiqueta.substr(0, 28);
        while (etiqueta.size() < 28) etiqueta += ' ';

        int pj = g.pgArr[idx] + g.peArr[idx] + g.ppArr[idx];
        salida << "\n   " << (i + 1) << "   " << etiqueta
               << "  " << pj
               << "   " << g.pgArr[idx]
               << "   " << g.peArr[idx]
               << "   " << g.ppArr[idx]
               << "   " << g.gfTabla[idx]
               << "   " << g.gcTabla[idx]
               << "  " << (g.dgTabla[idx] >= 0 ? "+" : "") << g.dgTabla[idx]
               << "    " << g.puntos[idx];
    }
    return salida;
}
