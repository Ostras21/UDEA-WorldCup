#ifndef ESTADISTICAJUGADOR_H
#define ESTADISTICAJUGADOR_H

#include <iostream>

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

    //  Getters
    int getPartidosJugados() const;
    int getGoles() const;
    int getMinutos() const;
    int getAsistencias() const;
    int getAmarillas() const;
    int getRojas() const;
    int getFaltas() const;

    // Setters (necesarios al reconstruir desde archivo)
    void setGoles(int goles);
    void setPartidosJugados(int p);

    // Metodo auxiliar: agrega manualmente los datos de un partido.
    // Se utilizara hasta que este disponible RegistroJugadorPartido.
    void acumularPartido(int golesPartido, int minutosPartido,
                         int amarillasPartido, int rojasPartido,
                         int faltasPartido, int asistenciasPartido);


    EstadisticaJugador& operator+=(const EstadisticaJugador& otra);

    // Impresion como funcion amiga.
    friend std::ostream& operator<<(std::ostream& salida, const EstadisticaJugador& e);
};

#endif // ESTADISTICAJUGADOR_H
