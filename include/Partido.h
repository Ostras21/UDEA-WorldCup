#ifndef PARTIDO_H
#define PARTIDO_H

#include <iostream>
#include <string>
#include "Fecha.h"
#include "Equipo.h"
#include "EstadisticaPartido.h"
#include "RegistroJugadorPartido.h"
#include "Lista.h"


class Partido {
private:
    Fecha fecha;
    std::string hora;        // ej: "15:00"
    std::string sede;        // string fijo segun decision
    std::string arbitros[3]; // ej: "codArbitro1", "codArbitro2", "codArbitro3"
    Equipo* equipo1;         // observador
    Equipo* equipo2;         // observador
    EstadisticaPartido estadistica1;
    EstadisticaPartido estadistica2;
    bool prorroga;           // true si fue a tiempo extra
    bool simulado;           // true tras llamar a simular()

public:
    Partido();
    Partido(const Fecha& fecha, const std::string& hora,
            const std::string& sede,
            Equipo* equipo1, Equipo* equipo2);

    //  Getters -
    Fecha       getFecha() const;
    std::string getHora()  const;
    std::string getSede()  const;
    std::string getArbitro(int indice) const;   // indice 0..2
    Equipo*     getEquipo1() const;
    Equipo*     getEquipo2() const;
    EstadisticaPartido&       getEstadistica1();
    EstadisticaPartido&       getEstadistica2();
    const EstadisticaPartido& getEstadistica1() const;
    const EstadisticaPartido& getEstadistica2() const;
    bool tuvoProrroga() const;
    bool fueSimulado()  const;

    //  Setters
    void setFecha(const Fecha& f);
    void setHora(const std::string& h);
    void setSede(const std::string& s);
    void setArbitro(int indice, const std::string& codigo);
    void setEquipo1(Equipo* e);
    void setEquipo2(Equipo* e);

    //  Simulacion
    // Si eliminatoria=true y el resultado es empate, se ejecuta
    // prorroga (ver doc-comment de la clase).
    void simular(bool eliminatoria);

    // ---------- Consultas tras simulacion ----------
    Equipo* getGanador()  const;   // nullptr si empate o sin simular
    bool    fueEmpate()   const;
    Lista<RegistroJugadorPartido> getGoleadores() const;

    friend std::ostream& operator<<(std::ostream& salida, const Partido& p);
};

#endif // PARTIDO_H
