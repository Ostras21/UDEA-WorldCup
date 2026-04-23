#include <iostream>
#include <string>

#include "Lista.h"
#include "Fecha.h"
#include "Confederacion.h"
#include "EstadisticaJugador.h"

/*
 * main.cpp
 * --------------------------------------------------------------
 * Programa de prueba para las clases implementadas hasta ahora:
 *   - Lista<T>           (plantilla propia con memoria dinamica)
 *   - Fecha              (aritmetica de dias, comparaciones)
 *   - Confederacion      (identificacion por codigo)
 *   - EstadisticaJugador (operator+= sobrecargado)
 *
 * El main definitivo desplegara el menu de funcionalidades del
 * torneo (carga de datos, sorteo, simulaciones, informe final,
 * medicion de recursos). Aqui solo se ejercitan las clases base.
 * --------------------------------------------------------------
 */

void imprimirTitulo(const std::string& titulo) {
    std::cout << "\n=====================================================\n";
    std::cout << " " << titulo << "\n";
    std::cout << "=====================================================\n";
}

// ---------- Pruebas de la plantilla Lista<T> ----------
void probarLista() {
    imprimirTitulo("Prueba de Lista<T>");

    Lista<int> numeros;
    for (int i = 1; i <= 5; ++i) {
        numeros.agregar(i * 10);
    }

    std::cout << "Tamano inicial: " << numeros.getTamano() << "\n";
    std::cout << "Elementos: ";
    for (int i = 0; i < numeros.getTamano(); ++i) {
        std::cout << numeros[i] << " ";
    }
    std::cout << "\n";

    // Probar eliminacion
    numeros.eliminar(2);
    std::cout << "Tras eliminar indice 2: ";
    for (int i = 0; i < numeros.getTamano(); ++i) {
        std::cout << numeros[i] << " ";
    }
    std::cout << "\n";

    // Probar constructor de copia (Regla de los Tres)
    Lista<int> copia(numeros);
    copia.agregar(999);
    std::cout << "Original tras copia: ";
    for (int i = 0; i < numeros.getTamano(); ++i) std::cout << numeros[i] << " ";
    std::cout << "\nCopia (con 999 anadido): ";
    for (int i = 0; i < copia.getTamano(); ++i) std::cout << copia[i] << " ";
    std::cout << "\n";

    // Probar operator=
    Lista<int> asignada;
    asignada = numeros;
    asignada.agregar(-1);
    std::cout << "Asignada (con -1 anadido): ";
    for (int i = 0; i < asignada.getTamano(); ++i) std::cout << asignada[i] << " ";
    std::cout << "\n";
    std::cout << "Original intacta: ";
    for (int i = 0; i < numeros.getTamano(); ++i) std::cout << numeros[i] << " ";
    std::cout << "\n";
}

//   Pruebas de Fecha
void probarFecha() {
    imprimirTitulo("Prueba de Fecha");

    Fecha inicioGrupos(20, 6, 2026);
    Fecha finGrupos(8, 7, 2026);

    std::cout << "Inicio fase de grupos: " << inicioGrupos << "\n";
    std::cout << "Fin fase de grupos:    " << finGrupos << "\n";
    std::cout << "Duracion (dias): " << finGrupos.diferenciaDias(inicioGrupos) << "\n";

    // Avanzar dias
    Fecha partido3 = inicioGrupos.avanzar(3);
    std::cout << "Inicio + 3 dias = " << partido3 << "\n";

    // Cruce de mes
    Fecha cruceMes = inicioGrupos.avanzar(15);
    std::cout << "Inicio + 15 dias = " << cruceMes << "\n";

    // Comparaciones
    std::cout << "inicio < fin ? " << (inicioGrupos < finGrupos ? "SI" : "NO") << "\n";
    std::cout << "inicio == (20/6/2026) ? "
              << (inicioGrupos == Fecha(20,6,2026) ? "SI" : "NO") << "\n";
}

// ---------- Pruebas de Confederacion ----------
void probarConfederacion() {
    imprimirTitulo("Prueba de Confederacion");

    Confederacion uefa("Union de Asociaciones Europeas de Futbol", "UEFA");
    Confederacion conmebol("Confederacion Sudamericana de Futbol", "CONMEBOL");
    Confederacion otraUefa("UEFA-duplicado", "UEFA");

    std::cout << "C1: " << uefa << "\n";
    std::cout << "C2: " << conmebol << "\n";
    std::cout << "uefa == conmebol ? " << (uefa == conmebol ? "SI" : "NO") << "\n";
    std::cout << "uefa == otraUefa ? " << (uefa == otraUefa ? "SI" : "NO")
              << " (comparacion por codigo)\n";
}

// ---------- Pruebas de EstadisticaJugador ----------
void probarEstadisticaJugador() {
    imprimirTitulo("Prueba de EstadisticaJugador");

    EstadisticaJugador historico;
    std::cout << "Historico inicial: " << historico << "\n";

    // Simular 3 partidos
    historico.acumularPartido(1, 90, 1, 0, 2, 0);  // partido 1
    historico.acumularPartido(2, 90, 0, 0, 1, 1);  // partido 2
    historico.acumularPartido(0, 120, 2, 1, 3, 0); // partido 3 con prorroga

    std::cout << "Tras 3 partidos:   " << historico << "\n";

    // Probar operator+=
    EstadisticaJugador otro(2, 3, 180, 1, 0, 0, 4);
    historico += otro;
    std::cout << "Tras += con otra estadistica: " << historico << "\n";
}

int main() {
    std::cout << "UdeAWorldCup - Prueba de clases base\n";

    probarLista();
    probarFecha();
    probarConfederacion();
    probarEstadisticaJugador();

    std::cout << "\nPruebas finalizadas correctamente.\n";
    return 0;
}
