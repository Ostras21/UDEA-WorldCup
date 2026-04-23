#ifndef ESTADISTICAJUGADOR_H
#define ESTADISTICAJUGADOR_H

#include <iostream>

/*
 * Clase EstadisticaJugador
 * --------------------------------------------------------------
 * Encapsula el historial de desempeño de un jugador:
 *   - partidos jugados
 *   - goles
 *   - minutos jugados
 *   - asistencias
 *   - tarjetas amarillas
 *   - tarjetas rojas
 *   - faltas cometidas
 *
 * Despues de cada partido de la Copa Mundial, estas estadisticas
 * deben actualizarse sumando los valores registrados en el
 * RegistroJugadorPartido correspondiente.
 *
 * Decisiones de diseño:
 *  - No maneja memoria dinamica -> no necesita Regla de los Tres.
 *  - Se sobrecarga operator+= para acumular resultados de un
 *    partido en el historico, lo que cumple el requisito de
 *    sobrecarga de operadores planteado en el enunciado.
 *  - operator<< se declara funcion amiga (requisito UML).
 *
 * NOTA: La clase RegistroJugadorPartido aun no existe en este
 * punto del desarrollo. Por eso el operator+= recibe por ahora
 * parametros primitivos (una version auxiliar). Cuando se
 * implemente RegistroJugadorPartido se añadira la sobrecarga
 * definitiva que recibe ese objeto.
 * --------------------------------------------------------------
 */
class EstadisticaJugador {
private:
    int partidosJugados;
    int goles;
    int minutos;
    int asistencias;
    int amarillas;
    int rojas;
    int faltas;

public:
    // Constructor por defecto: todos los contadores en cero.
    EstadisticaJugador();

    // Constructor con parametros (util al cargar desde archivo).
    EstadisticaJugador(int partidosJugados, int goles, int minutos,
                       int asistencias, int amarillas, int rojas, int faltas);

    // ---------- Getters ----------
    int getPartidosJugados() const;
    int getGoles() const;
    int getMinutos() const;
    int getAsistencias() const;
    int getAmarillas() const;
    int getRojas() const;
    int getFaltas() const;

    // ---------- Setters (necesarios al reconstruir desde archivo) ----------
    void setGoles(int goles);
    void setPartidosJugados(int p);

    // Metodo auxiliar: agrega manualmente los datos de un partido.
    // Se utilizara hasta que este disponible RegistroJugadorPartido.
    void acumularPartido(int golesPartido, int minutosPartido,
                         int amarillasPartido, int rojasPartido,
                         int faltasPartido, int asistenciasPartido);

    // Sobrecarga de operator+= que opera sobre otra EstadisticaJugador.
    // Esto permite fusionar dos historicos (por ejemplo, acumular
    // estadistica de partido a estadistica historica cuando ambas
    // se representan con la misma clase).
    EstadisticaJugador& operator+=(const EstadisticaJugador& otra);

    // Impresion como funcion amiga.
    friend std::ostream& operator<<(std::ostream& salida, const EstadisticaJugador& e);
};

#endif // ESTADISTICAJUGADOR_H
