#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <iostream>
#include <string>


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
