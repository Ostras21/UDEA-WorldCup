#include "Torneo.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <cstdlib>
#include <random>


static std::mt19937& rngTorneo() {
    static std::mt19937 g(std::random_device{}());
    return g;
}

// Devuelve la cadena en MAYUSCULAS (ASCII).
static std::string toUpperStr(const std::string& s) {
    std::string r = s;
    for (char& c : r) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return r;
}

// abierto en modo binario).
static std::string stripCR(const std::string& s) {
    if (!s.empty() && s.back() == '\r') return s.substr(0, s.size() - 1);
    return s;
}

static int splitCsv(const std::string& linea, char sep,
                    std::string out[], int max) {
    int count = 0;
    std::string acc;
    for (size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c == sep) {
            if (count < max) { out[count++] = acc; }
            acc.clear();
        } else {
            acc += c;
        }
    }
    if (count < max) { out[count++] = acc; }
    return count;
}

// ---------- Validaciones del sorteo de grupos ----------


static bool puedeAsignarseAlGrupo(const Grupo* g, const Equipo* e) {
    if (g == nullptr || e == nullptr) return false;
    Confederacion* confE = e->getConfederacion();
    if (confE == nullptr) return false;
    std::string codE = confE->getCodigo();
    bool esUEFA = (codE == "UEFA");
    int countUEFA = 0;
    const Lista<Equipo*>& eqs = g->getEquipos();
    for (int i = 0; i < eqs.getTamano(); ++i) {
        Confederacion* confI = eqs[i]->getConfederacion();
        if (confI == nullptr) continue;
        const std::string codI = confI->getCodigo();
        if (codI == "UEFA") ++countUEFA;
        if (!esUEFA && codI == codE) return false;
    }
    if (esUEFA && countUEFA >= 2) return false;
    return true;
}


static bool compatibleIgnorandoSlot(const Grupo* g, const Equipo* e,
                                    int slotIgnorar) {
    if (g == nullptr || e == nullptr) return false;
    Confederacion* confE = e->getConfederacion();
    if (confE == nullptr) return false;
    std::string codE = confE->getCodigo();
    bool esUEFA = (codE == "UEFA");
    int countUEFA = 0;
    const Lista<Equipo*>& eqs = g->getEquipos();
    for (int i = 0; i < eqs.getTamano(); ++i) {
        if (i == slotIgnorar) continue;
        Confederacion* confI = eqs[i]->getConfederacion();
        if (confI == nullptr) continue;
        const std::string codI = confI->getCodigo();
        if (codI == "UEFA") ++countUEFA;
        if (!esUEFA && codI == codE) return false;
    }
    if (esUEFA && countUEFA >= 2) return false;
    return true;
}


static void sortByPuntosDesc(Lista<Equipo*>& list, const Torneo* t) {
    int n = list.getTamano();
    if (n <= 1) return;

    Equipo* arr[16];
    int     tag[16];
    std::uniform_int_distribution<int> dist(0, 1000000);
    for (int i = 0; i < n; ++i) {
        arr[i] = list[i];
        tag[i] = dist(rngTorneo());   // etiqueta para sorteo de empates
    }

    for (int i = 1; i < n; ++i) {
        Equipo* clave = arr[i];
        int     tagK  = tag[i];
        int pK, dK, gK; t->statsTablaDe(clave, pK, dK, gK);
        int j = i - 1;
        while (j >= 0) {
            int pJ, dJ, gJ; t->statsTablaDe(arr[j], pJ, dJ, gJ);
            bool worse = false;
            if      (pJ < pK) worse = true;
            else if (pJ == pK && dJ < dK) worse = true;
            else if (pJ == pK && dJ == dK && gJ < gK) worse = true;
            else if (pJ == pK && dJ == dK && gJ == gK && tag[j] > tagK) worse = true;
            if (!worse) break;
            arr[j + 1] = arr[j];
            tag[j + 1] = tag[j];
            --j;
        }
        arr[j + 1] = clave;
        tag[j + 1] = tagK;
    }

    for (int i = 0; i < n; ++i) list[i] = arr[i];
}

// Empareja dos listas de igual tamaño n tal que ningun par comparta
// grupo de la fase anterior. Estrategia: shuffle de la lista B y
// validar todos los pares; reintentar hasta 200 veces. Devuelve los
// pares en outPairs[2*i] = A[i], outPairs[2*i+1] = match en B.
static bool emparejarBipartito(const Lista<Equipo*>& A,
                               const Lista<Equipo*>& B,
                               Equipo** outPairs, const Torneo* t) {
    int n = A.getTamano();
    if (n != B.getTamano()) return false;

    Equipo* arrB[16];
    for (int i = 0; i < n; ++i) arrB[i] = B[i];
    auto& rng = rngTorneo();

    for (int intento = 0; intento < 200; ++intento) {
        for (int i = n - 1; i > 0; --i) {
            std::uniform_int_distribution<int> d(0, i);
            int j = d(rng);
            Equipo* tmp = arrB[i]; arrB[i] = arrB[j]; arrB[j] = tmp;
        }
        bool ok = true;
        for (int i = 0; i < n && ok; ++i) {
            if (t->getGrupoDeEquipo(A[i]) == t->getGrupoDeEquipo(arrB[i])) ok = false;
        }
        if (ok) {
            for (int i = 0; i < n; ++i) {
                outPairs[2 * i]     = A[i];
                outPairs[2 * i + 1] = arrB[i];
            }
            return true;
        }
    }
    return false;
}

static bool emparejarMismaLista(const Lista<Equipo*>& L,
                                Equipo** outPairs, const Torneo* t) {
    int n = L.getTamano();
    if (n <= 0 || (n % 2) != 0) return false;

    Equipo* arr[16];
    for (int i = 0; i < n; ++i) arr[i] = L[i];
    auto& rng = rngTorneo();

    for (int intento = 0; intento < 200; ++intento) {
        for (int i = n - 1; i > 0; --i) {
            std::uniform_int_distribution<int> d(0, i);
            int j = d(rng);
            Equipo* tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
        bool ok = true;
        for (int i = 0; i < n && ok; i += 2) {
            if (t->getGrupoDeEquipo(arr[i]) == t->getGrupoDeEquipo(arr[i + 1])) ok = false;
        }
        if (ok) {
            for (int i = 0; i < n; ++i) outPairs[i] = arr[i];
            return true;
        }
    }
    return false;
}


static void imprimirPartidoElim(const Partido* p) {
    if (p == nullptr) return;
    std::cout << "  " << p->getEquipo1()->getCodigo() << "  "
              << p->getEstadistica1().getGolesFavor() << " - "
              << p->getEstadistica2().getGolesFavor() << "  "
              << p->getEquipo2()->getCodigo();
    if (p->tuvoProrroga()) std::cout << "  (PRORROGA)";
    std::cout << "\n    Posesion: "
              << p->getEquipo1()->getCodigo() << "="
              << p->getEstadistica1().getPosesion() << "%   "
              << p->getEquipo2()->getCodigo() << "="
              << p->getEstadistica2().getPosesion() << "%\n";

    std::cout << "    Goleadores: ";
    const Lista<RegistroJugadorPartido>& r1 = p->getEstadistica1().getRegistros();
    bool primero = true;
    for (int j = 0; j < r1.getTamano(); ++j) {
        int gj = r1[j].getGolesPartido();
        for (int k = 0; k < gj; ++k) {
            if (!primero) std::cout << ",";
            std::cout << "#" << r1[j].getJugador()->getNumeroCamiseta();
            primero = false;
        }
    }
    if (primero) std::cout << "-";
    std::cout << " / ";
    const Lista<RegistroJugadorPartido>& r2 = p->getEstadistica2().getRegistros();
    primero = true;
    for (int j = 0; j < r2.getTamano(); ++j) {
        int gj = r2[j].getGolesPartido();
        for (int k = 0; k < gj; ++k) {
            if (!primero) std::cout << ",";
            std::cout << "#" << r2[j].getJugador()->getNumeroCamiseta();
            primero = false;
        }
    }
    if (primero) std::cout << "-";
    std::cout << "\n";
}



// Codigo FIFA de 3 letras para el pais. Tabla manual; si el pais
// no esta en la tabla, se hace fallback con las 3 primeras letras
// en mayuscula sin espacios.
static std::string codigoFifaPais(const std::string& pais) {
    static const char* nombres[] = {
        "France","Spain","Argentina","England","Portugal","Brazil",
        "Netherlands","Belgium","Germany","Italy","Morocco","Uruguay",
        "Colombia","Croatia","Mexico","United States","Japan","Iran",
        "Senegal","Denmark","Austria","Korea Republic","Australia",
        "Turkey","Serbia","Sweden","Egypt","Canada","Paraguay","Panama",
        "Nigeria","Costa Rica","Tunisia","Czech Republic","Qatar",
        "Cameroon","Scotland","Uzbekistan","South Africa","Saudi Arabia",
        "Bosnia and Herzegovina","Iraq","Jordan","Ghana","DR Congo",
        "Haiti","New Zealand","Cabo Verde"
    };
    static const char* codigos[] = {
        "FRA","ESP","ARG","ENG","POR","BRA","NED","BEL","GER","ITA",
        "MAR","URU","COL","CRO","MEX","USA","JPN","IRN","SEN","DEN",
        "AUT","KOR","AUS","TUR","SRB","SWE","EGY","CAN","PAR","PAN",
        "NGA","CRC","TUN","CZE","QAT","CMR","SCO","UZB","RSA","KSA",
        "BIH","IRQ","JOR","GHA","COD","HAI","NZL","CPV"
    };
    static const int N = sizeof(nombres) / sizeof(nombres[0]);
    for (int i = 0; i < N; ++i) {
        if (pais == nombres[i]) return codigos[i];
    }
    // Fallback: 3 primeras letras MAYUSCULAS sin espacios.
    std::string r;
    for (size_t i = 0; i < pais.size() && r.size() < 3; ++i) {
        if (pais[i] == ' ') continue;
        r += static_cast<char>(std::toupper(static_cast<unsigned char>(pais[i])));
    }
    return r;
}

// ===============================================================


Torneo::Torneo()
    : confederaciones(), equipos(), grupos(), partidos(),
      partidosR16(), partidosR8(), partidosQF(), partidosSF(), partidosFinal(),
      bombos{Bombo(1), Bombo(2), Bombo(3), Bombo(4)},
      medidor(), campeon(nullptr) {}

Torneo::~Torneo() {
    // Liberar memoria dinamica en orden inverso a la dependencia:
    // partidos -> grupos -> equipos -> confederaciones.
    // (un Partido referencia Equipos; un Grupo referencia Equipos
    // y Partidos; un Equipo referencia una Confederacion.)
    for (int i = 0; i < partidos.getTamano(); ++i) {
        delete partidos[i];
    }
    for (int i = 0; i < grupos.getTamano(); ++i) {
        delete grupos[i];
    }
    for (int i = 0; i < equipos.getTamano(); ++i) {
        delete equipos[i];
    }
    for (int i = 0; i < confederaciones.getTamano(); ++i) {
        delete confederaciones[i];
    }
    // campeon es alias, no se libera aqui.
}

// ---------- Getters ----------
const Lista<Confederacion*>& Torneo::getConfederaciones() const { return confederaciones; }
const Lista<Equipo*>&        Torneo::getEquipos()        const { return equipos; }
const Lista<Grupo*>&         Torneo::getGrupos()         const { return grupos; }
const Lista<Partido*>&       Torneo::getPartidos()       const { return partidos; }
Lista<Equipo*>&              Torneo::getEquipos()              { return equipos; }
Lista<Grupo*>&               Torneo::getGrupos()               { return grupos; }
Lista<Partido*>&             Torneo::getPartidos()             { return partidos; }

MedidorRecursos&       Torneo::getMedidor()       { return medidor; }
const MedidorRecursos& Torneo::getMedidor() const { return medidor; }

const Bombo& Torneo::getBombo(int i) const {
    if (i < 0 || i > 3) {
        throw std::out_of_range("Torneo::getBombo: indice fuera de rango (0..3)");
    }
    return bombos[i];
}

const Lista<Partido*>& Torneo::getPartidosR16()   const { return partidosR16; }
const Lista<Partido*>& Torneo::getPartidosR8()    const { return partidosR8; }
const Lista<Partido*>& Torneo::getPartidosQF()    const { return partidosQF; }
const Lista<Partido*>& Torneo::getPartidosSF()    const { return partidosSF; }
const Lista<Partido*>& Torneo::getPartidosFinal() const { return partidosFinal; }

char Torneo::getGrupoDeEquipo(const Equipo* e) const {
    if (e == nullptr) return '?';
    for (int g = 0; g < grupos.getTamano(); ++g) {
        const Lista<Equipo*>& eqs = grupos[g]->getEquipos();
        for (int i = 0; i < eqs.getTamano(); ++i) {
            if (eqs[i] == e) return grupos[g]->getLetra();
        }
    }
    return '?';
}

void Torneo::statsTablaDe(const Equipo* e, int& puntos, int& dg, int& gf) const {
    puntos = 0; dg = 0; gf = 0;
    if (e == nullptr) return;
    for (int g = 0; g < grupos.getTamano(); ++g) {
        const Grupo* gr = grupos[g];
        for (int pos = 0; pos < gr->getEquipos().getTamano(); ++pos) {
            if (gr->getEquipoEnPosicion(pos) == e) {
                puntos = gr->getPuntosEnPosicion(pos);
                dg     = gr->getDGEnPosicion(pos);
                gf     = gr->getGFEnPosicion(pos);
                return;
            }
        }
    }
}

Equipo* Torneo::getCampeon() const { return campeon; }

// ---------- Funcionalidades ----------

Confederacion* Torneo::buscarConfederacionPorCodigo(const std::string& codigo) const {
    for (int i = 0; i < confederaciones.getTamano(); ++i) {
        if (confederaciones[i]->getCodigo() == codigo) {
            return confederaciones[i];
        }
    }
    return nullptr;
}

void Torneo::cargarDatos(const std::string& rutaCsv) {
    // Apertura en BINARIO para detectar BOM byte a byte y conservar
    // los \r (que limpiamos manualmente con stripCR).
    std::ifstream ifs(rutaCsv, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + rutaCsv);
    }

    // Detectar y saltar BOM UTF-8 (EF BB BF).
    char b1 = 0, b2 = 0, b3 = 0;
    ifs.get(b1); ifs.get(b2); ifs.get(b3);
    bool hayBom = (static_cast<unsigned char>(b1) == 0xEF &&
                   static_cast<unsigned char>(b2) == 0xBB &&
                   static_cast<unsigned char>(b3) == 0xBF);
    if (!hayBom) {
        ifs.clear();
        ifs.seekg(0);
    }

    // Saltar las 2 primeras lineas (titulo + encabezados de columnas).
    std::string linea;
    for (int i = 0; i < 2; ++i) {
        if (!std::getline(ifs, linea)) {
            throw std::runtime_error("CSV truncado: faltan lineas de cabecera");
        }
    }

    // Procesar lineas de datos. El CSV trae 48 equipos.
    while (std::getline(ifs, linea)) {
        linea = stripCR(linea);
        if (linea.empty()) continue;          // saltar lineas vacias finales
        if (linea[0] == ';') continue;        // saltar separadores fantasma

        std::string campos[10];
        int n = splitCsv(linea, ';', campos, 10);
        if (n < 10) continue;                 // linea malformada -> ignorar
        if (campos[0].empty()) continue;      // sin ranking -> ignorar

        int   ranking      = std::stoi(campos[0]);
        const std::string& pais        = campos[1];
        const std::string& director    = campos[2];
        // campos[3] = federacion (no se almacena; el UML no la tiene)
        std::string codigoConf         = toUpperStr(campos[4]);
        int   golesFavor   = std::stoi(campos[5]);
        int   golesContra  = std::stoi(campos[6]);
        int   ganados      = std::stoi(campos[7]);
        int   empatados    = std::stoi(campos[8]);
        int   perdidos     = std::stoi(campos[9]);

        // Reusar o crear la confederacion (clave: codigo en MAYUSCULAS).
        Confederacion* conf = buscarConfederacionPorCodigo(codigoConf);
        if (conf == nullptr) {
            conf = new Confederacion(codigoConf, codigoConf);
            confederaciones.agregar(conf);
            medidor.registrarMemoria(sizeof(Confederacion));
        }

        // Crear equipo y volcar estadistica historica del CSV.
        std::string codigoPais = codigoFifaPais(pais);
        Equipo* equipo = new Equipo(pais, codigoPais, director, ranking, conf);
        EstadisticaEquipo est(golesFavor, golesContra,
                              ganados, empatados, perdidos,
                              0, 0, 0);   // amarillas/rojas/faltas en cero
        equipo->setEstadistica(est);

        equipos.agregar(equipo);
        medidor.registrarMemoria(sizeof(Equipo));

        // Fabrica los 26 jugadores del equipo con el reparto de
        // goles historicos definido por el enunciado (PASO 2.3).
        fabricarJugadores(equipo);

        medidor.registrarIteracion();
    }

    medidor.reportar("Carga de datos");
}

void Torneo::fabricarJugadores(Equipo* e) {
    if (e == nullptr) return;

    // Reparto uniforme de los goles historicos del equipo entre las
    // 26 camisetas. Si la division no es exacta, los primeros
    // 'golesExtra' jugadores reciben un gol adicional para que la
    // suma cuadre exactamente con golesFavor del equipo.
    int golesEquipo = e->getEstadistica().getGolesFavor();
    int golesBase   = golesEquipo / 26;
    int golesExtra  = golesEquipo % 26;

    for (int i = 1; i <= 26; ++i) {
        int golesAsignados = golesBase + (i <= golesExtra ? 1 : 0);

        Jugador j("nombre" + std::to_string(i),
                  "apellido" + std::to_string(i),
                  i);
        // Constructor de EstadisticaJugador: PJ, G, Min, As, Am, Ro, Fal.
        // Todo en cero salvo los goles asignados (regla del enunciado).
        EstadisticaJugador est(0, golesAsignados, 0, 0, 0, 0, 0);
        j.setEstadistica(est);

        e->agregarJugador(j);
        medidor.registrarIteracion();
        medidor.registrarMemoria(sizeof(Jugador));
    }
}

void Torneo::conformarBombos() {
    // 1) Localizar al anfitrion (USA).
    Equipo* usa = nullptr;
    for (int i = 0; i < equipos.getTamano(); ++i) {
        if (equipos[i]->getCodigo() == "USA") { usa = equipos[i]; break; }
    }
    if (usa == nullptr) {
        throw std::runtime_error("conformarBombos: equipo anfitrion (USA) no encontrado");
    }

    // 2) USA va al Bombo 1.
    bombos[0].agregar(usa);

    // 3) Lista temporal con los 47 equipos restantes.
    Lista<Equipo*> resto;
    for (int i = 0; i < equipos.getTamano(); ++i) {
        if (equipos[i] != usa) {
            resto.agregar(equipos[i]);
            medidor.registrarIteracion();
        }
    }

    // 4) Insertion sort por ranking ASC (n=47 -> ~2200 comparaciones
    //    en peor caso, despreciable). Se elige insertion sort por ser
    //    estable, en-place y trivial de auditar; quicksort propio
    //    seria innecesariamente complejo a esta escala. std::sort
    //    esta prohibido por el enunciado.
    int n = resto.getTamano();
    for (int i = 1; i < n; ++i) {
        Equipo* clave = resto[i];
        int j = i - 1;
        while (j >= 0 && resto[j]->getRankingFIFA() > clave->getRankingFIFA()) {
            resto[j + 1] = resto[j];
            --j;
            medidor.registrarIteracion();
        }
        resto[j + 1] = clave;
        medidor.registrarIteracion();
    }

    // 5) Distribuir: 0..10 -> bombo 1 (junto a USA), 11..22 -> bombo 2,
    //    23..34 -> bombo 3, 35..46 -> bombo 4.
    for (int i = 0; i <= 10 && i < n; ++i) {
        bombos[0].agregar(resto[i]);
        medidor.registrarIteracion();
    }
    for (int i = 11; i <= 22 && i < n; ++i) {
        bombos[1].agregar(resto[i]);
        medidor.registrarIteracion();
    }
    for (int i = 23; i <= 34 && i < n; ++i) {
        bombos[2].agregar(resto[i]);
        medidor.registrarIteracion();
    }
    for (int i = 35; i <= 46 && i < n; ++i) {
        bombos[3].agregar(resto[i]);
        medidor.registrarIteracion();
    }
}

void Torneo::sortearGrupos() {
    medidor.resetIteraciones();

    // Crear los 12 grupos A..L (vacios) y registrarlos en el medidor.
    for (int i = 0; i < 12; ++i) {
        Grupo* g = new Grupo(static_cast<char>('A' + i));
        grupos.agregar(g);
        medidor.registrarMemoria(sizeof(Grupo));
    }

    // Para cada grupo (A..L) y cada bombo (0..3), extraer un equipo
    // valido. Si en 200 intentos no se logra, intentar swap con un
    // grupo previo.
    const int MAX_INTENTOS = 200;

    for (int gi = 0; gi < grupos.getTamano(); ++gi) {
        Grupo* g = grupos[gi];

        for (int b = 0; b < 4; ++b) {
            Bombo& bombo = bombos[b];
            Equipo* asignado = nullptr;

            // ---- Intento directo: extraer-validar-devolver ----
            for (int t = 0; t < MAX_INTENTOS && !bombo.vacio(); ++t) {
                Equipo* candidato = bombo.extraerAleatorio();
                medidor.registrarIteracion();
                if (puedeAsignarseAlGrupo(g, candidato)) {
                    asignado = candidato;
                    break;
                }
                bombo.devolver(candidato);
            }

            // ---- Fallback: swap con un grupo anterior ----
            if (asignado == nullptr) {
                Lista<Equipo*> remanentes;
                while (!bombo.vacio()) {
                    remanentes.agregar(bombo.extraerAleatorio());
                }

                for (int prev = 0; prev < gi && asignado == nullptr; ++prev) {
                    Grupo* gPrev = grupos[prev];
                    Lista<Equipo*>& eqsPrev = gPrev->getEquipos();
                    if (eqsPrev.getTamano() <= b) continue;

                    Equipo* prevTeam = eqsPrev[b];
                    if (!puedeAsignarseAlGrupo(g, prevTeam)) continue;

                    int idxSwap = -1;
                    for (int r = 0; r < remanentes.getTamano(); ++r) {
                        if (compatibleIgnorandoSlot(gPrev, remanentes[r], b)) {
                            idxSwap = r;
                            break;
                        }
                    }
                    if (idxSwap < 0) continue;

                    // Ejecutar swap: prevTeam -> grupo actual,
                    // remanentes[idxSwap] -> grupo previo (reemplaza el slot b).
                    eqsPrev[b] = remanentes[idxSwap];
                    asignado = prevTeam;
                    remanentes.eliminar(idxSwap);
                    medidor.registrarIteracion();
                }

                // Devolver al bombo lo que no se uso en el swap.
                for (int r = 0; r < remanentes.getTamano(); ++r) {
                    bombo.agregar(remanentes[r]);
                }

                if (asignado == nullptr) {
                    throw std::runtime_error(
                        "sortearGrupos: imposible cumplir restricciones (incluso tras swap)");
                }
            }

            g->agregarEquipo(asignado);
        }
    }

    // Imprimir los 12 grupos con la confederacion de cada equipo
    // (requisito explicito del enunciado).
    std::cout << "\n--- Grupos conformados ---\n";
    for (int i = 0; i < grupos.getTamano(); ++i) {
        const Grupo* g = grupos[i];
        std::cout << "Grupo " << g->getLetra() << ":\n";
        const Lista<Equipo*>& eqs = g->getEquipos();
        for (int j = 0; j < eqs.getTamano(); ++j) {
            const Equipo* eq = eqs[j];
            std::cout << "  " << eq->getCodigo()
                      << "  " << eq->getPais()
                      << "  (rank " << eq->getRankingFIFA()
                      << ", " << eq->getConfederacion()->getCodigo() << ")\n";
        }
    }

    medidor.reportar("Sorteo de grupos");
}

void Torneo::generarPartidosFaseGrupos() {
    // Para cada grupo, construir las 6 combinaciones (i,j) con i<j
    // sobre los 4 equipos. Se crean 12 grupos * 6 partidos = 72.
    for (int g = 0; g < grupos.getTamano(); ++g) {
        Grupo* grupo = grupos[g];
        const Lista<Equipo*>& eqs = grupo->getEquipos();
        for (int a = 0; a < eqs.getTamano(); ++a) {
            for (int b = a + 1; b < eqs.getTamano(); ++b) {
                Partido* p = new Partido(Fecha(),         // placeholder, se asigna luego
                                         "00:00",
                                         "nombreSede",
                                         eqs[a], eqs[b]);
                grupo->agregarPartido(p);
                partidos.agregar(p);
                medidor.registrarMemoria(sizeof(Partido));
                medidor.registrarIteracion();
            }
        }
    }
}

void Torneo::asignarFechasFaseGrupos() {
    const Fecha base(20, 6, 2026);   // dia 0
    const int   VENTANA = 19;        // 20/06 .. 08/07/2026
    const int   MAX_POR_DIA = 4;
    const int   SEPARACION_MIN = 3;  // minimos 3 dias entre partidos del mismo equipo

    int contadorDia[VENTANA];
    for (int i = 0; i < VENTANA; ++i) contadorDia[i] = 0;

    int relajaciones = 0;

    for (int i = 0; i < partidos.getTamano(); ++i) {
        Partido* p = partidos[i];
        Equipo*  e1 = p->getEquipo1();
        Equipo*  e2 = p->getEquipo2();

        int diaElegido = -1;

        // Buscar primera fecha valida en el rango.
        for (int d = 0; d < VENTANA; ++d) {
            if (contadorDia[d] >= MAX_POR_DIA) continue;

            bool conflicto = false;
            for (int j = 0; j < i && !conflicto; ++j) {
                Partido* pj = partidos[j];
                bool involucraEquipo =
                    (pj->getEquipo1() == e1 || pj->getEquipo2() == e1 ||
                     pj->getEquipo1() == e2 || pj->getEquipo2() == e2);
                if (!involucraEquipo) continue;
                int dj = pj->getFecha().diferenciaDias(base);
                if (std::abs(d - dj) < SEPARACION_MIN) conflicto = true;
            }
            if (!conflicto) { diaElegido = d; break; }

            medidor.registrarIteracion();
        }

        // Relajacion documentada: si no hubo fecha valida, tomar la
        // primera con cupo (<4 partidos) aunque rompa la separacion.
        if (diaElegido < 0) {
            for (int d = 0; d < VENTANA; ++d) {
                if (contadorDia[d] < MAX_POR_DIA) { diaElegido = d; break; }
            }
            if (diaElegido < 0) {
                throw std::runtime_error(
                    "asignarFechasFaseGrupos: rango 19 dias agotado");
            }
            ++relajaciones;
            std::cout << "  [relajacion] " << e1->getCodigo()
                      << " vs " << e2->getCodigo()
                      << " asignado a " << base.avanzar(diaElegido)
                      << " sin respetar separacion minima\n";
        }

        p->setFecha(base.avanzar(diaElegido));
        ++contadorDia[diaElegido];
    }

    std::cout << "Relajaciones aplicadas: " << relajaciones << "\n";
}

void Torneo::simularFaseGrupos() {
    medidor.resetIteraciones();

    // 1) Simular los 72 partidos. Una sola vez cada uno; el orden de
    //    this->partidos refleja el orden de inscripcion en grupos.
    for (int i = 0; i < partidos.getTamano(); ++i) {
        partidos[i]->simular(false);
        medidor.registrarIteracion();
    }

    // 2) Calcular tabla de cada grupo (ordena posicion[] internamente).
    for (int g = 0; g < grupos.getTamano(); ++g) {
        grupos[g]->calcularTabla();
        medidor.registrarIteracion();
    }

    medidor.reportar("Simulacion fase de grupos");
}

void Torneo::clasificarYEmparejar() {
    medidor.resetIteraciones();

    // 1) Recopilar primeros, segundos y terceros (12 cada uno).
    Lista<Equipo*> primeros, segundos, terceros;
    for (int g = 0; g < grupos.getTamano(); ++g) {
        const Grupo* gr = grupos[g];
        if (gr->getEquipos().getTamano() >= 1) primeros.agregar(gr->getEquipoEnPosicion(0));
        if (gr->getEquipos().getTamano() >= 2) segundos.agregar(gr->getEquipoEnPosicion(1));
        if (gr->getEquipos().getTamano() >= 3) terceros.agregar(gr->getEquipoEnPosicion(2));
        medidor.registrarIteracion();
    }

    // 2) Ordenar las 3 listas: puntos DESC -> DG DESC -> GF DESC -> sorteo.
    sortByPuntosDesc(primeros, this);
    sortByPuntosDesc(segundos, this);
    sortByPuntosDesc(terceros, this);

    // 3) Particionar.
    //    Primeros:   top 8 (rule 1) + bottom 4 (rule 2).
    //    Segundos:   top 8 (rule 3 entre si) + bottom 4 (rule 2).
    //    Terceros:   top 8 (rule 1, clasifican)  + bottom 4 (eliminados).
    Lista<Equipo*> primerosTop8, primerosBot4;
    Lista<Equipo*> segundosTop8, segundosBot4;
    Lista<Equipo*> mejoresTerceros;
    for (int i = 0; i < primeros.getTamano(); ++i) {
        if (i < 8) primerosTop8.agregar(primeros[i]);
        else       primerosBot4.agregar(primeros[i]);
    }
    for (int i = 0; i < segundos.getTamano(); ++i) {
        if (i < 8) segundosTop8.agregar(segundos[i]);
        else       segundosBot4.agregar(segundos[i]);
    }
    for (int i = 0; i < 8 && i < terceros.getTamano(); ++i) {
        mejoresTerceros.agregar(terceros[i]);
    }

    // 4) Emparejar las 3 reglas (shuffle con max 200 intentos).
    Equipo* pairs1[16];   // 8 pares: top cabezas vs mejores terceros
    Equipo* pairs2[8];    // 4 pares: bottom cabezas vs peores segundos
    Equipo* pairs3[8];    // 4 pares: top segundos entre si

    if (!emparejarBipartito(primerosTop8, mejoresTerceros, pairs1, this)) {
        throw std::runtime_error("clasificarYEmparejar: imposible emparejar regla 1");
    }
    medidor.registrarIteracion();
    if (!emparejarBipartito(primerosBot4, segundosBot4, pairs2, this)) {
        throw std::runtime_error("clasificarYEmparejar: imposible emparejar regla 2");
    }
    medidor.registrarIteracion();
    if (!emparejarMismaLista(segundosTop8, pairs3, this)) {
        throw std::runtime_error("clasificarYEmparejar: imposible emparejar regla 3");
    }
    medidor.registrarIteracion();

    // 5) Crear los 16 Partido R16 con fecha 10/07/2026.
    Fecha fechaR16(10, 7, 2026);
    auto crearPartido = [&](Equipo* a, Equipo* b) {
        Partido* p = new Partido(fechaR16, "00:00", "nombreSede", a, b);
        partidos.agregar(p);
        partidosR16.agregar(p);
        medidor.registrarMemoria(sizeof(Partido));
        medidor.registrarIteracion();
    };
    for (int i = 0; i < 8; ++i) crearPartido(pairs1[2 * i], pairs1[2 * i + 1]);
    for (int i = 0; i < 4; ++i) crearPartido(pairs2[2 * i], pairs2[2 * i + 1]);
    for (int i = 0; i < 4; ++i) crearPartido(pairs3[2 * i], pairs3[2 * i + 1]);

    medidor.reportar("Clasificacion y emparejamiento R16");
}

void Torneo::simularEliminatoria() {
    medidor.resetIteraciones();
    const Fecha fechaElim(10, 7, 2026);

    // Lambda local: crea un Partido eliminatorio y lo registra tanto
    // en 'partidos' (dueno de la memoria) como en la lista alias por
    // etapa que se le pase.
    auto crearYRegistrar = [&](Equipo* a, Equipo* b,
                               Lista<Partido*>& alias) -> Partido* {
        Partido* p = new Partido(fechaElim, "00:00", "nombreSede", a, b);
        partidos.agregar(p);
        alias.agregar(p);
        medidor.registrarMemoria(sizeof(Partido));
        return p;
    };

    //  DIECISEISAVOS (R16, ya creados)
    std::cout << "\n=== DIECISEISAVOS DE FINAL ===\n";
    Lista<Equipo*> ganadoresR16;
    for (int i = 0; i < partidosR16.getTamano(); ++i) {
        Partido* p = partidosR16[i];
        p->simular(true);
        imprimirPartidoElim(p);
        ganadoresR16.agregar(p->getGanador());
        medidor.registrarIteracion();
    }

    //  OCTAVOS (R8)
    std::cout << "\n=== OCTAVOS DE FINAL ===\n";
    Lista<Equipo*> ganadoresR8;
    for (int i = 0; i + 1 < ganadoresR16.getTamano(); i += 2) {
        Partido* p = crearYRegistrar(ganadoresR16[i], ganadoresR16[i + 1], partidosR8);
        p->simular(true);
        imprimirPartidoElim(p);
        ganadoresR8.agregar(p->getGanador());
        medidor.registrarIteracion();
    }

    //  CUARTOS (QF)
    std::cout << "\n=== CUARTOS DE FINAL ===\n";
    Lista<Equipo*> ganadoresQF;
    for (int i = 0; i + 1 < ganadoresR8.getTamano(); i += 2) {
        Partido* p = crearYRegistrar(ganadoresR8[i], ganadoresR8[i + 1], partidosQF);
        p->simular(true);
        imprimirPartidoElim(p);
        ganadoresQF.agregar(p->getGanador());
        medidor.registrarIteracion();
    }

    //  SEMIFINALES (SF)
    std::cout << "\n=== SEMIFINALES ===\n";
    Lista<Equipo*> ganadoresSF;
    Lista<Equipo*> perdedoresSF;
    for (int i = 0; i + 1 < ganadoresQF.getTamano(); i += 2) {
        Partido* p = crearYRegistrar(ganadoresQF[i], ganadoresQF[i + 1], partidosSF);
        p->simular(true);
        imprimirPartidoElim(p);
        Equipo* g = p->getGanador();
        Equipo* l = (g == p->getEquipo1()) ? p->getEquipo2() : p->getEquipo1();
        ganadoresSF.agregar(g);
        perdedoresSF.agregar(l);
        medidor.registrarIteracion();
    }
    if (ganadoresSF.getTamano() < 2 || perdedoresSF.getTamano() < 2) {
        throw std::runtime_error(
            "simularEliminatoria: SF no produjo 2 ganadores y 2 perdedores");
    }

    //  TERCER PUESTO + FINAL
    std::cout << "\n=== TERCER PUESTO ===\n";
    Partido* p3 = crearYRegistrar(perdedoresSF[0], perdedoresSF[1], partidosFinal);
    p3->simular(true);
    imprimirPartidoElim(p3);
    medidor.registrarIteracion();

    std::cout << "\n=== FINAL DEL MUNDIAL ===\n";
    Partido* pFin = crearYRegistrar(ganadoresSF[0], ganadoresSF[1], partidosFinal);
    pFin->simular(true);
    imprimirPartidoElim(pFin);
    medidor.registrarIteracion();

    campeon = pFin->getGanador();

    medidor.reportar("Simulacion eliminatorias");
}

void Torneo::generarInformeFinal() {
    medidor.resetIteraciones();

    std::cout << "\n=====================================================\n";
    std::cout << "          INFORME ESTADISTICO FINAL\n";
    std::cout << "=====================================================\n";

    if (campeon == nullptr || partidosFinal.getTamano() < 2) {
        std::cout << "[INFORME] El torneo aun no se ha completado.\n";
        medidor.reportar("Generacion de informe final");
        return;
    }

    //  Top 4
    Partido* pFin  = partidosFinal[1];
    Partido* p3rd  = partidosFinal[0];
    Equipo*  pri   = campeon;
    Equipo*  seg   = (pri == pFin->getEquipo1()) ? pFin->getEquipo2() : pFin->getEquipo1();
    Equipo*  ter   = p3rd->getGanador();
    Equipo*  cua   = (ter == p3rd->getEquipo1()) ? p3rd->getEquipo2() : p3rd->getEquipo1();

    std::cout << "\n--- 1. Ranking Top 4 del Mundial ---\n";
    std::cout << "  1. " << pri->getPais() << " (" << pri->getCodigo() << ")\n";
    std::cout << "  2. " << seg->getPais() << " (" << seg->getCodigo() << ")\n";
    std::cout << "  3. " << ter->getPais() << " (" << ter->getCodigo() << ")\n";
    std::cout << "  4. " << cua->getPais() << " (" << cua->getCodigo() << ")\n";
    medidor.registrarIteracion();

    // Goleador del campeon
    std::cout << "\n--- 2. Maximo goleador del equipo campeon ---\n";
    {
        const Lista<Jugador>& plant = pri->getPlantilla();
        int idxMax = -1, golesMax = -1;
        for (int i = 0; i < plant.getTamano(); ++i) {
            int g = plant[i].getEstadistica().getGoles();
            if (g > golesMax) { golesMax = g; idxMax = i; }
            medidor.registrarIteracion();
        }
        if (idxMax >= 0) {
            const Jugador& j = plant[idxMax];
            std::cout << "  #" << j.getNumeroCamiseta() << " "
                      << j.getNombre() << " " << j.getApellido()
                      << " (" << pri->getCodigo() << "): "
                      << golesMax << " goles historicos\n";
        }
    }

    // Top 3 goleadores del torneo (por goles HISTORICOS finales).
    std::cout << "\n--- 3. Tres mayores goleadores de toda la copa ---\n";
    const Jugador* top3[3]      = {nullptr, nullptr, nullptr};
    int            top3G[3]     = {-1, -1, -1};
    Equipo*        top3Eq[3]    = {nullptr, nullptr, nullptr};
    for (int e = 0; e < equipos.getTamano(); ++e) {
        Equipo* eq = equipos[e];
        const Lista<Jugador>& pl = eq->getPlantilla();
        for (int j = 0; j < pl.getTamano(); ++j) {
            int g = pl[j].getEstadistica().getGoles();
            if (g <= top3G[2]) { medidor.registrarIteracion(); continue; }
            int slot = (g > top3G[0]) ? 0 : (g > top3G[1]) ? 1 : 2;
            for (int k = 2; k > slot; --k) {
                top3[k]   = top3[k - 1];
                top3G[k]  = top3G[k - 1];
                top3Eq[k] = top3Eq[k - 1];
            }
            top3[slot]   = &pl[j];
            top3G[slot]  = g;
            top3Eq[slot] = eq;
            medidor.registrarIteracion();
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (top3[i] == nullptr) continue;
        std::cout << "  " << (i + 1) << ". #" << top3[i]->getNumeroCamiseta()
                  << " " << top3[i]->getNombre() << " " << top3[i]->getApellido()
                  << " (" << top3Eq[i]->getCodigo() << "): "
                  << top3G[i] << " goles\n";
    }

    // Equipo con mas goles historicos (post-torneo).
    std::cout << "\n--- 4. Equipo con mas goles historicos (post-torneo) ---\n";
    Equipo* maxEq = nullptr;
    int maxGF = -1;
    for (int i = 0; i < equipos.getTamano(); ++i) {
        int gf = equipos[i]->getEstadistica().getGolesFavor();
        if (gf > maxGF) { maxGF = gf; maxEq = equipos[i]; }
        medidor.registrarIteracion();
    }
    if (maxEq != nullptr) {
        std::cout << "  " << maxEq->getPais() << " (" << maxEq->getCodigo()
                  << ") con " << maxGF << " goles a favor\n";
    }

    // Confederacion mas presente en R16, R8 y SF (R4).
    std::cout << "\n--- 5. Confederacion con mayor presencia por etapa ---\n";
    auto confTop = [&](const Lista<Partido*>& etapa) -> Confederacion* {
        int n = confederaciones.getTamano();
        if (n <= 0) return nullptr;
        if (n > 32) n = 32;
        int conteo[32];
        for (int i = 0; i < n; ++i) conteo[i] = 0;
        for (int p = 0; p < etapa.getTamano(); ++p) {
            Equipo* arrEq[2] = {etapa[p]->getEquipo1(), etapa[p]->getEquipo2()};
            for (int s = 0; s < 2; ++s) {
                Confederacion* c = arrEq[s] ? arrEq[s]->getConfederacion() : nullptr;
                if (c == nullptr) continue;
                for (int i = 0; i < n; ++i) {
                    if (confederaciones[i] == c) { ++conteo[i]; break; }
                }
            }
            medidor.registrarIteracion();
        }
        int maxIdx = 0;
        for (int i = 1; i < n; ++i) if (conteo[i] > conteo[maxIdx]) maxIdx = i;
        return confederaciones[maxIdx];
    };
    Confederacion* c16 = confTop(partidosR16);
    Confederacion* c8  = confTop(partidosR8);
    Confederacion* c4  = confTop(partidosSF);
    if (c16) std::cout << "  R16:    " << c16->getCodigo() << "\n";
    if (c8)  std::cout << "  R8:     " << c8->getCodigo()  << "\n";
    if (c4)  std::cout << "  SF/R4:  " << c4->getCodigo()  << "\n";

    medidor.reportar("Generacion de informe final");
}

void Torneo::persistirJugadores(const std::string& rutaCsv) {
    std::ofstream ofs(rutaCsv);
    if (!ofs.is_open()) {
        throw std::runtime_error("No se pudo abrir archivo de salida: " + rutaCsv);
    }

    int lineas = 0;
    for (int e = 0; e < equipos.getTamano(); ++e) {
        const Equipo* eq = equipos[e];
        const Lista<Jugador>& pl = eq->getPlantilla();
        for (int j = 0; j < pl.getTamano(); ++j) {
            const Jugador& jug = pl[j];
            const EstadisticaJugador& est = jug.getEstadistica();
            ofs << eq->getCodigo()       << ';'
                << jug.getNumeroCamiseta() << ';'
                << jug.getNombre()         << ';'
                << jug.getApellido()       << ';'
                << est.getPartidosJugados()<< ';'
                << est.getGoles()          << ';'
                << est.getMinutos()        << ';'
                << est.getAsistencias()    << ';'
                << est.getAmarillas()      << ';'
                << est.getRojas()          << ';'
                << est.getFaltas()         << '\n';
            ++lineas;
            medidor.registrarIteracion();
        }
    }
    ofs.close();
    std::cout << "[persistirJugadores] " << lineas << " lineas escritas en "
              << rutaCsv << "\n";
    medidor.reportar("Persistencia de jugadores");
}
