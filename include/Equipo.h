#ifndef EQUIPO_H
#define EQUIPO_H

#include <string>
#include <iostream>
#include "Lista.h"
#include "Jugador.h"
#include "EstadisticaEquipo.h"
#include "Confederacion.h"


class Equipo {
private:
    std::string pais;
    std::string codigo;        // codigo corto del pais
    std::string director;
    int rankingFIFA;
    Confederacion* confederacion; // observador, no se destruye aqui
    Lista<Jugador> plantilla;
    EstadisticaEquipo estadistica;

public:
    Equipo();
    Equipo(const std::string& pais, const std::string& codigo,
           const std::string& director, int rankingFIFA,
           Confederacion* confederacion);

    // ---------- Getters ----------
    std::string getPais()      const;
    std::string getCodigo()    const;
    std::string getDirector()  const;
    int         getRankingFIFA() const;
    Confederacion* getConfederacion() const;

    Lista<Jugador>&        getPlantilla();
    const Lista<Jugador>&  getPlantilla() const;
    EstadisticaEquipo&     getEstadistica();
    const EstadisticaEquipo& getEstadistica() const;

    // ---------- Setters ----------
    void setPais(const std::string& v);
    void setCodigo(const std::string& v);
    void setDirector(const std::string& v);
    void setRankingFIFA(int v);
    void setConfederacion(Confederacion* c);
    void setEstadistica(const EstadisticaEquipo& e);

    // ---------- Operaciones de plantilla ----------
    void agregarJugador(const Jugador& j);
    int  cantidadJugadores() const;

    // Devuelve el jugador con el numero de camiseta indicado, por
    // referencia para permitir actualizaciones de su estadistica.
    // Lanza out_of_range si no existe.
    Jugador& buscarJugadorPorCamiseta(int numero);

    // ---------- Operadores requeridos ----------
    // Ordena por rankingFIFA ascendente (mejor ranking primero).
    bool operator<(const Equipo& otro) const;

    // Compara por codigo de pais (identidad).
    bool operator==(const Equipo& otro) const;

    friend std::ostream& operator<<(std::ostream& salida, const Equipo& e);
};

#endif // EQUIPO_H
