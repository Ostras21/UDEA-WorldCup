#ifndef ESTADISTICAEQUIPO_H
#define ESTADISTICAEQUIPO_H

#include <iostream>

/*
 * Clase EstadisticaEquipo
 * --------------------------------------------------------------
 * Encapsula el historial estadistico de un equipo:
 *   - goles a favor / en contra
 *   - partidos ganados / empatados / perdidos
 *   - tarjetas amarillas / rojas
 *   - faltas
 *
 * Despues de cada partido del torneo se acumulan los datos del
 * encuentro al historico mediante operator+= o el metodo
 * acumularPartido (este ultimo se usa hasta que exista
 * EstadisticaPartido como clase aparte).
 *
 * Decisiones de diseño:
 *  - No maneja memoria dinamica.
 *  - Sobrecarga operator+= (otro requisito de operadores cumplido).
 *  - operator<< como funcion amiga.
 *  - Provee promedios goleadores que el Torneo necesita para
 *    aplicar la ecuacion (1) de simulacion de partidos.
 * --------------------------------------------------------------
 */
class EstadisticaEquipo {
private:
    int golesFavor;
    int golesContra;
    int ganados;
    int empatados;
    int perdidos;
    int amarillas;
    int rojas;
    int faltas;

public:
    EstadisticaEquipo();
    EstadisticaEquipo(int gf, int gc, int g, int e, int p,
                      int am, int ro, int fa);

    // ---------- Getters ----------
    int getGolesFavor()  const;
    int getGolesContra() const;
    int getGanados()     const;
    int getEmpatados()   const;
    int getPerdidos()    const;
    int getAmarillas()   const;
    int getRojas()       const;
    int getFaltas()      const;

    // Cantidad total de partidos historicos (suma de g/e/p).
    int getPartidosJugados() const;

    // Puntos acumulados (3 por victoria, 1 por empate).
    int getPuntos() const;

    // Diferencia de goles (GF - GC).
    int getDiferenciaGoles() const;

    // Promedios usados por la formula de simulacion.
    // Si el equipo no tiene historico, devuelve 1.0 como neutro.
    double promedioGolesFavor()  const;
    double promedioGolesContra() const;

    // ---------- Setters (para carga desde CSV) ----------
    void setGolesFavor(int v);
    void setGolesContra(int v);
    void setGanados(int v);
    void setEmpatados(int v);
    void setPerdidos(int v);

    // ---------- Acumulacion ----------
    // Acumula los datos de un partido individual al historico.
    // Resultado: 'V' (victoria), 'E' (empate), 'D' (derrota).
    void acumularPartido(int golesFavorPart, int golesContraPart,
                         int amarillasPart, int rojasPart,
                         int faltasPart, char resultado);

    // Sobrecarga: fusiona dos estadisticas (suma campo a campo).
    EstadisticaEquipo& operator+=(const EstadisticaEquipo& otra);

    friend std::ostream& operator<<(std::ostream& salida, const EstadisticaEquipo& e);
};

#endif // ESTADISTICAEQUIPO_H
