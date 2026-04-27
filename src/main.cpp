#include <iostream>
#include <string>
#include <stdexcept>

#include "Lista.h"
#include "Fecha.h"
#include "Confederacion.h"
#include "EstadisticaJugador.h"
#include "EstadisticaEquipo.h"
#include "Jugador.h"
#include "Equipo.h"
#include "MedidorRecursos.h"
#include "RegistroJugadorPartido.h"
#include "EstadisticaPartido.h"
#include "Bombo.h"
#include "Partido.h"
#include "Grupo.h"
#include "Torneo.h"


static void imprimirMenu() {

    std::cout << "   UdeAWorldCup - Menu\n";
    std::cout << "-------------------------------'\n";
    std::cout << "1. Cargar / actualizar datos\n";
    std::cout << "2. Conformar grupos\n";
    std::cout << "3. Simular fase de grupos y eliminatorias\n";
    std::cout << "4. Generar informe estadistico final\n";
    std::cout << "5. Mostrar consumo de recursos del ultimo bloque\n";
    std::cout << "6. Persistir estadisticas a archivo\n";
    std::cout << "7. Salir\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "Opcion: ";
}

int main() {
    Torneo torneo;
    bool datosCargados     = false;
    bool gruposConformados = false;
    bool partidosSimulados = false;

    bool salir = false;
    while (!salir) {
        imprimirMenu();

        int opcion = 0;
        if (!(std::cin >> opcion)) {
            std::cout << "\n[fin de entrada] saliendo.\n";
            break;
        }

        try {
            switch (opcion) {
                case 1: {
                    torneo.cargarDatos("data/selecciones_clasificadas_mundial.csv");
                    datosCargados = true;
                    std::cout << "OK Cargados "
                              << torneo.getEquipos().getTamano() << " equipos y "
                              << torneo.getConfederaciones().getTamano()
                              << " confederaciones.\n";
                    break;
                }
                case 2: {
                    if (!datosCargados) {
                        std::cout << "REQUISITO Primero ejecute la opcion 1 (cargar datos).\n";
                        break;
                    }
                    torneo.conformarBombos();
                    torneo.sortearGrupos();
                    gruposConformados = true;
                    break;
                }
                case 3: {
                    if (!gruposConformados) {
                        std::cout << "REQUISITO Primero ejecute la opcion 2 (conformar grupos).\n";
                        break;
                    }
                    torneo.generarPartidosFaseGrupos();
                    torneo.asignarFechasFaseGrupos();
                    torneo.simularFaseGrupos();
                    torneo.clasificarYEmparejar();
                    torneo.simularEliminatoria();
                    partidosSimulados = true;
                    break;
                }
                case 4: {
                    if (!partidosSimulados) {
                        std::cout << "REQUISITO Primero ejecute la opcion 3 (simular).\n";
                        break;
                    }
                    torneo.generarInformeFinal();
                    break;
                }
                case 5: {
                    torneo.getMedidor().reportar("Estado actual del medidor");
                    break;
                }
                case 6: {
                    if (!partidosSimulados) {
                        std::cout << "REQUISITO Primero ejecute la opcion 3 (simular).\n";
                        break;
                    }
                    std::string ruta = "data/jugadores_finales.csv";
                    torneo.persistirJugadores(ruta);
                    break;
                }
                case 7: {
                    salir = true;
                    break;
                }
                default: {
                    std::cout << "ERROR Opcion invalida (use 1..7).\n";
                    break;
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "EXCEPCION " << ex.what() << "\n";
        }
    }

    std::cout << "\nHasta luego.\n";
    return 0;
}
