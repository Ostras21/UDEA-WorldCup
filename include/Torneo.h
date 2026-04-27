#ifndef TORNEO_H
#define TORNEO_H

#include <string>
#include "Lista.h"
#include "Confederacion.h"
#include "Equipo.h"
#include "Grupo.h"
#include "Partido.h"
#include "Bombo.h"
#include "MedidorRecursos.h"


class Torneo {
private:
    Lista<Confederacion*> confederaciones;
    Lista<Equipo*>        equipos;
    Lista<Grupo*>         grupos;
    Lista<Partido*>       partidos;       // dueno: libera todos en el dtor
    // Listas alias por etapa eliminatoria (los Partido* tambien estan
    // en 'partidos', dueno unico de la memoria).
    Lista<Partido*>       partidosR16;
    Lista<Partido*>       partidosR8;
    Lista<Partido*>       partidosQF;
    Lista<Partido*>       partidosSF;
    Lista<Partido*>       partidosFinal;  // 2 entradas: [0]=tercer puesto, [1]=final
    Bombo                 bombos[4];      // bombos 1..4
    MedidorRecursos       medidor;
    Equipo*               campeon;        // observador, no se destruye

    // Helper privado: construye los 26 jugadores artificiales del
    // equipo segun reglas del enunciado. Implementacion en PASO 2.3.
    void fabricarJugadores(Equipo* e);

    Confederacion* buscarConfederacionPorCodigo(const std::string& codigo) const;


public:
    Torneo();
    ~Torneo();


    Torneo(const Torneo&) = delete;
    Torneo& operator=(const Torneo&) = delete;

    // ---------- Getters ----------
    const Lista<Confederacion*>& getConfederaciones() const;
    const Lista<Equipo*>&        getEquipos()        const;
    const Lista<Grupo*>&         getGrupos()         const;
    const Lista<Partido*>&       getPartidos()       const;
    Lista<Equipo*>&              getEquipos();
    Lista<Grupo*>&               getGrupos();
    Lista<Partido*>&             getPartidos();

    MedidorRecursos&       getMedidor();
    const MedidorRecursos& getMedidor() const;

    // Acceso de solo lectura a un bombo (i en 0..3) para auditoria.
    const Bombo& getBombo(int i) const;

    const Lista<Partido*>& getPartidosR16()   const;
    const Lista<Partido*>& getPartidosR8()    const;
    const Lista<Partido*>& getPartidosQF()    const;
    const Lista<Partido*>& getPartidosSF()    const;
    const Lista<Partido*>& getPartidosFinal() const;


    char getGrupoDeEquipo(const Equipo* e) const;


    void statsTablaDe(const Equipo* e, int& puntos, int& dg, int& gf) const;

    Equipo* getCampeon() const;

    // ---------- Funcionalidades del enunciado ----------

    // Funcionalidad I (parte): carga los 48 equipos desde el CSV.
    void cargarDatos(const std::string& rutaCsv);

    // Funcionalidad II: arma los 4 bombos a partir del ranking FIFA.
    void conformarBombos();

    // Funcionalidad II: ejecuta el sorteo aleatorio respetando
    // restriccion de confederaciones (max 2 UEFA por grupo).
    void sortearGrupos();

    void generarPartidosFaseGrupos();
    void asignarFechasFaseGrupos();

    // Funcionalidad III (a): simula los 6 partidos de cada grupo.
    void simularFaseGrupos();

    void clasificarYEmparejar();

    // Funcionalidad III (c): simula R32, R16, cuartos, semis y final.
    void simularEliminatoria();

    void generarInformeFinal();

    void persistirJugadores(const std::string& rutaCsv);
};

#endif // TORNEO_H
