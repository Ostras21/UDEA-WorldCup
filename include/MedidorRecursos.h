#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <iostream>
#include <string>

/*
 * Clase MedidorRecursos
 * --------------------------------------------------------------
 * Cumple el Requisito Funcional V del enunciado: medicion del
 * consumo de recursos. Lleva dos contadores:
 *   a) Iteraciones realizadas durante una funcionalidad.
 *   b) Memoria estimada actualmente en uso por las estructuras
 *      dinamicas del sistema.
 *
 * Uso previsto:
 *   - Cada clase con memoria dinamica notifica al medidor cuando
 *     reserva o libera memoria.
 *   - Los bucles criticos llaman a registrarIteracion().
 *   - Al finalizar una funcionalidad, se llama a reportar() y
 *     se reinician los contadores de iteraciones (la memoria NO
 *     se reinicia, refleja el estado actual del sistema).
 *
 * Decisiones de diseño:
 *  - long permite contadores grandes (104 partidos x 11 jugadores
 *    x N sub-eventos puede dispararse rapido).
 *  - La memoria es una APROXIMACION basada en sizeof; el enunciado
 *    pide un valor estimado, no exacto.
 *  - No maneja memoria dinamica propia.
 * --------------------------------------------------------------
 */
class MedidorRecursos {
private:
    long iteraciones;     // contador de iteraciones de la funcionalidad actual
    long memoriaBytes;    // total estimado de memoria viva en bytes

public:
    MedidorRecursos();

    // ---------- Iteraciones ----------
    void registrarIteracion();
    void registrarIteraciones(long n); // util para sumar lotes
    long getIteraciones() const;
    void resetIteraciones();

    // ---------- Memoria ----------
    void registrarMemoria(long bytes);   // suma al total
    void liberarMemoria(long bytes);     // resta del total
    long getMemoriaBytes() const;

    // Reporte legible en pantalla. Recibe nombre de la funcionalidad
    // para etiquetar la salida.
    void reportar(const std::string& funcionalidad) const;
};

#endif // MEDIDORRECURSOS_H
