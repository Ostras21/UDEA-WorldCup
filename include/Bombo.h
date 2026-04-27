#ifndef BOMBO_H
#define BOMBO_H

#include <iostream>
#include "Lista.h"
#include "Equipo.h"

/*
 * Clase Bombo
 * --------------------------------------------------------------
 * Contenedor temporal de equipos usado durante el sorteo de
 * grupos (Funcionalidad II). El torneo crea 4 bombos:
 *   - Bombo 1: anfitrion (EE.UU.) + 11 mejores del ranking
 *   - Bombo 2: equipos 13..24
 *   - Bombo 3: equipos 25..36
 *   - Bombo 4: equipos 37..48
 *
 * El sorteo extrae aleatoriamente un equipo de cada bombo y lo
 * asigna a un grupo. Si el equipo extraido viola las restricciones
 * de confederacion, se devuelve al bombo y se intenta otro.
 *
 * Decisiones de diseño:
 *  - Almacena Equipo* (agregacion): los equipos viven en el Torneo,
 *    el Bombo solo los referencia durante el sorteo.
 *  - extraerAleatorio() devuelve un puntero y elimina el equipo
 *    del bombo. devolver() lo regresa al final.
 *  - El RNG es un std::mt19937 con seed de std::random_device,
 *    estatico dentro del .cpp para no exponer dependencias en .h.
 *  - operator<< como funcion amiga (lista los equipos restantes).
 * --------------------------------------------------------------
 */
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
