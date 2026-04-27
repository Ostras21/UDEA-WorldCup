#include "Partido.h"
#include <stdexcept>
#include <random>
#include <cmath>

/*
 * Nota de implementacion: el flag 'simulado' NO aparece en el
 * diagrama UML; es una decision interna para evitar simular dos
 * veces el mismo partido (por ejemplo, si el Torneo recorre la
 * lista de partidos mas de una vez por error). Tambien permite
 * que getGanador()/fueEmpate() distingan "empate real 0-0" de
 * "partido aun no jugado".
 */

// =============================================================
// RNG y helpers internos para la simulacion (PASO 4.2)
// =============================================================

// RNG estatico compartido (random_device como en Bombo). Si en
// Bloque 4 surge la necesidad de seed configurable para depurar,
// se cambia aqui en un solo punto.
static std::mt19937& rngPartido() {
    static std::mt19937 g(std::random_device{}());
    return g;
}

// Formula del enunciado:
//   lambda = mu * (gf/mu)^alpha * (gc/mu)^beta
// con mu=1.35, alpha=0.6, beta=0.4. Si los promedios vienen 0
// (equipo sin historico) la EstadisticaEquipo ya devuelve 1.0
// como valor neutro, asi que aqui no hay que protegerse.
static double calcularLambda(double gf, double gc) {
    const double mu = 1.35, alpha = 0.6, beta = 0.4;
    return mu * std::pow(gf / mu, alpha) * std::pow(gc / mu, beta);
}

// Selecciona hasta 11 jugadores al azar (Fisher-Yates) sin
// repeticion de la plantilla. Si la plantilla tiene menos de 11
// jugadores (caso defensivo, en el flujo real son 26), convoca
// los disponibles. Devuelve la cantidad real de convocados.
static int seleccionarConvocados(Lista<Jugador>& plantilla,
                                 Jugador* convocados[11]) {
    int n = plantilla.getTamano();
    if (n <= 0) return 0;
    int cap = (n < 26) ? n : 26;
    int idx[26];
    for (int i = 0; i < cap; ++i) idx[i] = i;
    for (int i = cap - 1; i > 0; --i) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(rngPartido());
        int tmp = idx[i]; idx[i] = idx[j]; idx[j] = tmp;
    }
    int convocar = (cap < 11) ? cap : 11;
    for (int i = 0; i < convocar; ++i) {
        convocados[i] = &plantilla[idx[i]];
    }
    return convocar;
}

// Simula los goles de un equipo dado su lambda (numero esperado).
// Recorre los 'n' jugadores convocados con probabilidad 4% cada uno
// hasta agotar round(lambda) goles o llegar al limite de iteraciones
// (proteccion frente a lambdas grandes). Resetea golesPorJug a 0
// antes de empezar (para reuso en prorrogas).
static int simularGolesEquipo(double lambda, int n, int golesPorJug[11]) {
    for (int i = 0; i < 11; ++i) golesPorJug[i] = 0;
    int objetivo = static_cast<int>(std::round(lambda));
    if (objetivo < 0) objetivo = 0;
    if (n <= 0)       return 0;

    std::uniform_real_distribution<double> uni(0.0, 1.0);
    auto& rng = rngPartido();

    int marcados = 0;
    const int LIMITE = 200;
    int it = 0;
    while (marcados < objetivo && it < LIMITE) {
        for (int i = 0; i < n && marcados < objetivo; ++i) {
            if (uni(rng) < 0.04) {
                ++golesPorJug[i];
                ++marcados;
            }
        }
        ++it;
    }
    return marcados;
}

Partido::Partido()
    : fecha(), hora(""), sede("nombreSede"),
      equipo1(nullptr), equipo2(nullptr),
      estadistica1(), estadistica2(),
      prorroga(false), simulado(false) {
    arbitros[0] = "codArbitro1";
    arbitros[1] = "codArbitro2";
    arbitros[2] = "codArbitro3";
}

Partido::Partido(const Fecha& fecha, const std::string& hora,
                 const std::string& sede,
                 Equipo* equipo1, Equipo* equipo2)
    : fecha(fecha), hora(hora), sede(sede),
      equipo1(equipo1), equipo2(equipo2),
      estadistica1(), estadistica2(),
      prorroga(false), simulado(false) {
    arbitros[0] = "codArbitro1";
    arbitros[1] = "codArbitro2";
    arbitros[2] = "codArbitro3";
}

// ---------- Getters ----------
Fecha       Partido::getFecha() const { return fecha; }
std::string Partido::getHora()  const { return hora; }
std::string Partido::getSede()  const { return sede; }

std::string Partido::getArbitro(int indice) const {
    if (indice < 0 || indice > 2) {
        throw std::out_of_range("Partido::getArbitro: indice fuera de rango (0..2)");
    }
    return arbitros[indice];
}

Equipo* Partido::getEquipo1() const { return equipo1; }
Equipo* Partido::getEquipo2() const { return equipo2; }

EstadisticaPartido&       Partido::getEstadistica1()       { return estadistica1; }
EstadisticaPartido&       Partido::getEstadistica2()       { return estadistica2; }
const EstadisticaPartido& Partido::getEstadistica1() const { return estadistica1; }
const EstadisticaPartido& Partido::getEstadistica2() const { return estadistica2; }

bool Partido::tuvoProrroga() const { return prorroga; }
bool Partido::fueSimulado()  const { return simulado; }

//  Setters
void Partido::setFecha(const Fecha& f)         { fecha = f; }
void Partido::setHora(const std::string& h)    { hora = h; }
void Partido::setSede(const std::string& s)    { sede = s; }

void Partido::setArbitro(int indice, const std::string& codigo) {
    if (indice < 0 || indice > 2) {
        throw std::out_of_range("Partido::setArbitro: indice fuera de rango (0..2)");
    }
    arbitros[indice] = codigo;
}

void Partido::setEquipo1(Equipo* e) { equipo1 = e; }
void Partido::setEquipo2(Equipo* e) { equipo2 = e; }

// Simulacion
void Partido::simular(bool eliminatoria) {
    if (equipo1 == nullptr || equipo2 == nullptr) {
        throw std::runtime_error("Partido::simular: equipos no asignados");
    }

    double gfA = equipo1->getEstadistica().promedioGolesFavor();
    double gcA = equipo1->getEstadistica().promedioGolesContra();
    double gfB = equipo2->getEstadistica().promedioGolesFavor();
    double gcB = equipo2->getEstadistica().promedioGolesContra();
    double lambdaA = calcularLambda(gfA, gcB);
    double lambdaB = calcularLambda(gfB, gcA);

    Jugador* conv1[11];
    Jugador* conv2[11];
    int n1 = seleccionarConvocados(equipo1->getPlantilla(), conv1);
    int n2 = seleccionarConvocados(equipo2->getPlantilla(), conv2);

    std::uniform_real_distribution<double> uni(0.0, 1.0);
    auto& rng = rngPartido();


    int faltas1[11]    = {0}; int faltas2[11]    = {0};
    int amarillas1[11] = {0}; int amarillas2[11] = {0};
    int rojas1[11]     = {0}; int rojas2[11]     = {0};

    for (int i = 0; i < n1; ++i) {
        // Faltas: 13% / 2.75% / 0.7% (max 3).
        if (uni(rng) < 0.13)                      faltas1[i] = 1;
        if (faltas1[i] == 1 && uni(rng) < 0.0275) faltas1[i] = 2;
        if (faltas1[i] == 2 && uni(rng) < 0.007)  faltas1[i] = 3;
        // Amarillas: 6% / 1.15%; dos amarillas suman 1 roja.
        if (uni(rng) < 0.06)                          amarillas1[i] = 1;
        if (amarillas1[i] == 1 && uni(rng) < 0.0115) {
            amarillas1[i] = 2;
            rojas1[i] = 1;
        }
    }
    for (int i = 0; i < n2; ++i) {
        if (uni(rng) < 0.13)                      faltas2[i] = 1;
        if (faltas2[i] == 1 && uni(rng) < 0.0275) faltas2[i] = 2;
        if (faltas2[i] == 2 && uni(rng) < 0.007)  faltas2[i] = 3;
        if (uni(rng) < 0.06)                          amarillas2[i] = 1;
        if (amarillas2[i] == 1 && uni(rng) < 0.0115) {
            amarillas2[i] = 2;
            rojas2[i] = 1;
        }
    }

    // Goles del 90'.
    int goles1[11] = {0};
    int goles2[11] = {0};
    int totalA = simularGolesEquipo(lambdaA, n1, goles1);
    int totalB = simularGolesEquipo(lambdaB, n2, goles2);


    prorroga = false;
    int minutosBase = 90;
    if (eliminatoria && totalA == totalB) {
        prorroga = true;
        minutosBase = 120;
        // El equipo de mejor ranking (numero menor) recibe el +1.
        bool mejorEsA = (equipo1->getRankingFIFA() <= equipo2->getRankingFIFA());
        bool roto = false;
        for (int intento = 1; intento <= 5 && !roto; ++intento) {
            double lA = lambdaA + (mejorEsA ? intento : 0);
            double lB = lambdaB + (mejorEsA ? 0       : intento);
            totalA = simularGolesEquipo(lA, n1, goles1);
            totalB = simularGolesEquipo(lB, n2, goles2);
            if (totalA != totalB) roto = true;
        }

        if (!roto) {
            if (mejorEsA && n1 > 0) { ++goles1[0]; ++totalA; }
            else if (n2 > 0)        { ++goles2[0]; ++totalB; }
        }
    }

    int minutos1[11], minutos2[11];
    for (int i = 0; i < 11; ++i) {
        minutos1[i] = minutosBase;
        minutos2[i] = minutosBase;
    }


    double rA = static_cast<double>(equipo1->getRankingFIFA());
    double rB = static_cast<double>(equipo2->getRankingFIFA());
    double posA = 50.0, posB = 50.0;
    if (rA + rB > 0) {
        posA = (rB / (rA + rB)) * 100.0;
        posB = 100.0 - posA;
    }


    estadistica1.getRegistros().limpiar();
    estadistica2.getRegistros().limpiar();
    int totAm1 = 0, totRo1 = 0, totFa1 = 0;
    int totAm2 = 0, totRo2 = 0, totFa2 = 0;
    for (int i = 0; i < n1; ++i) {
        estadistica1.agregarRegistro(
            RegistroJugadorPartido(conv1[i], goles1[i], minutos1[i],
                                   amarillas1[i], rojas1[i], faltas1[i], 0));
        totAm1 += amarillas1[i]; totRo1 += rojas1[i]; totFa1 += faltas1[i];
    }
    for (int i = 0; i < n2; ++i) {
        estadistica2.agregarRegistro(
            RegistroJugadorPartido(conv2[i], goles2[i], minutos2[i],
                                   amarillas2[i], rojas2[i], faltas2[i], 0));
        totAm2 += amarillas2[i]; totRo2 += rojas2[i]; totFa2 += faltas2[i];
    }
    estadistica1.setGolesFavor(totalA);
    estadistica1.setGolesContra(totalB);
    estadistica1.setPosesion(posA);
    estadistica2.setGolesFavor(totalB);
    estadistica2.setGolesContra(totalA);
    estadistica2.setPosesion(posB);


    for (int i = 0; i < n1; ++i) {
        conv1[i]->actualizar(goles1[i], minutos1[i],
                             amarillas1[i], rojas1[i], faltas1[i], 0);
    }
    for (int i = 0; i < n2; ++i) {
        conv2[i]->actualizar(goles2[i], minutos2[i],
                             amarillas2[i], rojas2[i], faltas2[i], 0);
    }


    char res1 = (totalA > totalB) ? 'V' : (totalA < totalB) ? 'D' : 'E';
    char res2 = (totalB > totalA) ? 'V' : (totalB < totalA) ? 'D' : 'E';
    equipo1->getEstadistica().acumularPartido(totalA, totalB,
                                              totAm1, totRo1, totFa1, res1);
    equipo2->getEstadistica().acumularPartido(totalB, totalA,
                                              totAm2, totRo2, totFa2, res2);

    simulado = true;
}

// Consultas
Equipo* Partido::getGanador() const {
    if (!simulado) return nullptr;
    int gf1 = estadistica1.getGolesFavor();
    int gf2 = estadistica2.getGolesFavor();
    if (gf1 > gf2) return equipo1;
    if (gf2 > gf1) return equipo2;
    return nullptr;
}

bool Partido::fueEmpate() const {
    if (!simulado) return false;
    return estadistica1.getGolesFavor() == estadistica2.getGolesFavor();
}

Lista<RegistroJugadorPartido> Partido::getGoleadores() const {
    Lista<RegistroJugadorPartido> goleadores;
    const Lista<RegistroJugadorPartido>& reg1 = estadistica1.getRegistros();
    for (int i = 0; i < reg1.getTamano(); ++i) {
        if (reg1[i].getGolesPartido() > 0) goleadores.agregar(reg1[i]);
    }
    const Lista<RegistroJugadorPartido>& reg2 = estadistica2.getRegistros();
    for (int i = 0; i < reg2.getTamano(); ++i) {
        if (reg2[i].getGolesPartido() > 0) goleadores.agregar(reg2[i]);
    }
    return goleadores;
}

std::ostream& operator<<(std::ostream& salida, const Partido& p) {
    salida << p.fecha << " " << p.hora << " @ " << p.sede << " | ";
    if (p.equipo1 != nullptr) salida << p.equipo1->getCodigo();
    else salida << "???";
    salida << " " << p.estadistica1.getGolesFavor()
           << " - " << p.estadistica2.getGolesFavor() << " ";
    if (p.equipo2 != nullptr) salida << p.equipo2->getCodigo();
    else salida << "???";
    if (p.prorroga) salida << " (prorroga)";
    if (!p.simulado) salida << " [pendiente]";
    return salida;
}
