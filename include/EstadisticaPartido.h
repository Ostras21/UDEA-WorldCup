#ifndef ESTADISTICAPARTIDO_H
#define ESTADISTICAPARTIDO_H

#include <iostream>
#include "Lista.h"
#include "RegistroJugadorPartido.h"


class EstadisticaPartido {
private:
    Lista<RegistroJugadorPartido> registros;
    int golesFavor;
    int golesContra;
    double posesion;   // porcentaje 0..100

public:
    EstadisticaPartido();
    EstadisticaPartido(int golesFavor, int golesContra, double posesion);

    //  Getters
    Lista<RegistroJugadorPartido>&        getRegistros();
    const Lista<RegistroJugadorPartido>&  getRegistros() const;
    int    getGolesFavor()  const;
    int    getGolesContra() const;
    double getPosesion()    const;

    //  Setters
    void setGolesFavor(int v);
    void setGolesContra(int v);
    void setPosesion(double v);

    //  Operaciones
    void agregarRegistro(const RegistroJugadorPartido& r);
    int  cantidadRegistros() const;

    friend std::ostream& operator<<(std::ostream& salida, const EstadisticaPartido& e);
};

#endif // ESTADISTICAPARTIDO_H
