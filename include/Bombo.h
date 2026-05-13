#ifndef BOMBO_H
#define BOMBO_H

#include <iostream>
#include "Lista.h"
#include "Equipo.h"


class Bombo {
private:
    int numero;              // 1, 2, 3 o 4
    Lista<Equipo*> equipos;  // observadores

public:
    Bombo();
    Bombo(int numero);

    // ---------- Getters ----------
    int getNumero() const;
    int cantidad()  const;
    Lista<Equipo*>&        getEquipos();
    const Lista<Equipo*>&  getEquipos() const;

    // ---------- Setters ----------
    void setNumero(int n);

    // ---------- Operaciones de sorteo ----------
    // Agrega un equipo al bombo.
    void agregar(Equipo* e);

    // Extrae un equipo aleatorio del bombo eliminandolo de la lista.
    // Retorna nullptr si el bombo esta vacio.
    Equipo* extraerAleatorio();

    // Devuelve un equipo previamente extraido al final del bombo.
    void devolver(Equipo* e);

    // Indica si el bombo esta vacio.
    bool vacio() const;

    friend std::ostream& operator<<(std::ostream& salida, const Bombo& b);
};

#endif // BOMBO_H
