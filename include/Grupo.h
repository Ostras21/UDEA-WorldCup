#ifndef GRUPO_H
#define GRUPO_H

#include <iostream>
#include "Lista.h"
#include "Equipo.h"

class Partido;
class Grupo {
private:
    char letra;                    // 'A'..'L'
    Lista<Equipo*>  equipos;       // observadores, max 4
    Lista<Partido*> partidos;      // observadores, max 6


    int puntos[4];
    int gfTabla[4];
    int gcTabla[4];
    int dgTabla[4];
    int pgArr[4];                  // partidos ganados
    int peArr[4];                  // partidos empatados
    int ppArr[4];                  // partidos perdidos
    int posicion[4];               // ranking final (indices a equipos[])
    bool tablaCalculada;

public:
    Grupo();
    Grupo(char letra);

    // ---------- Getters ----------
    char getLetra() const;
    Lista<Equipo*>&        getEquipos();
    const Lista<Equipo*>&  getEquipos() const;
    Lista<Partido*>&       getPartidos();
    const Lista<Partido*>& getPartidos() const;
    int cantidadEquipos()  const;
    int cantidadPartidos() const;

    // ---------- Setters ----------
    void setLetra(char l);

    // ---------- Composicion ----------
    void agregarEquipo(Equipo* e);     // ignora si ya hay 4
    void agregarPartido(Partido* p);   // ignora si ya hay 6

    // ---------- Tabla ----------
    // Recalcula puntos, GF, GC, DG y orden a partir de los partidos
    // simulados del grupo. Llamar tras simular toda la fase.
    void calcularTabla();

    // Devuelve los dos primeros de la tabla (clasificados directos).
    Lista<Equipo*> getClasificados() const;

    // Devuelve el equipo que termino tercero (candidato a mejor 3o).
    Equipo* getTercero() const;

    // Acceso por posicion final (0=primero, 3=ultimo).
    Equipo* getEquipoEnPosicion(int pos) const;
    int     getPuntosEnPosicion(int pos) const;
    int     getDGEnPosicion(int pos) const;
    int     getGFEnPosicion(int pos) const;

    friend std::ostream& operator<<(std::ostream& salida, const Grupo& g);
};

#endif // GRUPO_H
