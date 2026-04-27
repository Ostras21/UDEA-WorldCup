#include "Equipo.h"
#include <stdexcept>

Equipo::Equipo()
    : pais(""), codigo(""), director(""), rankingFIFA(0),
      confederacion(nullptr), plantilla(), estadistica() {}

Equipo::Equipo(const std::string& pais, const std::string& codigo,
               const std::string& director, int rankingFIFA,
               Confederacion* confederacion)
    : pais(pais), codigo(codigo), director(director),
      rankingFIFA(rankingFIFA), confederacion(confederacion),
      plantilla(), estadistica() {}

// ---------- Getters ----------
std::string Equipo::getPais()        const { return pais; }
std::string Equipo::getCodigo()      const { return codigo; }
std::string Equipo::getDirector()    const { return director; }
int         Equipo::getRankingFIFA() const { return rankingFIFA; }
Confederacion* Equipo::getConfederacion() const { return confederacion; }

Lista<Jugador>&       Equipo::getPlantilla()       { return plantilla; }
const Lista<Jugador>& Equipo::getPlantilla() const { return plantilla; }
EstadisticaEquipo&    Equipo::getEstadistica()       { return estadistica; }
const EstadisticaEquipo& Equipo::getEstadistica() const { return estadistica; }

// ---------- Setters ----------
void Equipo::setPais(const std::string& v)         { pais = v; }
void Equipo::setCodigo(const std::string& v)       { codigo = v; }
void Equipo::setDirector(const std::string& v)     { director = v; }
void Equipo::setRankingFIFA(int v)                 { rankingFIFA = v; }
void Equipo::setConfederacion(Confederacion* c)    { confederacion = c; }
void Equipo::setEstadistica(const EstadisticaEquipo& e) { estadistica = e; }

// ---------- Operaciones plantilla ----------
void Equipo::agregarJugador(const Jugador& j) {
    plantilla.agregar(j);
}

int Equipo::cantidadJugadores() const {
    return plantilla.getTamano();
}

Jugador& Equipo::buscarJugadorPorCamiseta(int numero) {
    for (int i = 0; i < plantilla.getTamano(); ++i) {
        if (plantilla[i].getNumeroCamiseta() == numero) {
            return plantilla[i];
        }
    }
    throw std::out_of_range("Equipo::buscarJugadorPorCamiseta: numero no encontrado");
}

// ---------- Operadores ----------
bool Equipo::operator<(const Equipo& otro) const {
    // Menor ranking FIFA = mejor equipo. Se ordena ascendente.
    return rankingFIFA < otro.rankingFIFA;
}

bool Equipo::operator==(const Equipo& otro) const {
    // Identidad por codigo de pais.
    return codigo == otro.codigo;
}

std::ostream& operator<<(std::ostream& salida, const Equipo& e) {
    salida << e.pais << " (" << e.codigo << ")"
           << " [Rank #" << e.rankingFIFA << "]"
           << " DT: " << e.director;
    if (e.confederacion != nullptr) {
        salida << " - " << e.confederacion->getCodigo();
    }
    salida << " | Plantilla: " << e.plantilla.getTamano() << " jugadores";
    return salida;
}
