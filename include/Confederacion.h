#ifndef CONFEDERACION_H
#define CONFEDERACION_H

#include <string>
#include <iostream>

/*
 * Clase Confederacion
 * --------------------------------------------------------------
 * Agrupa a los equipos por ubicacion geografica (UEFA, CONMEBOL,
 * CONCACAF, CAF, AFC, OFC).
 *
 * Responsabilidad principal:
 *  - Identificar la confederacion a la que pertenece un equipo.
 *  - Permite aplicar las restricciones del sorteo de grupos:
 *    no se permiten dos selecciones de la misma confederacion
 *    en el mismo grupo, salvo UEFA (maximo dos).
 *
 * Decisiones de diseño:
 *  - No posee memoria dinamica -> no requiere Regla de los Tres.
 *  - Se incluye operator== para comparar confederaciones por codigo
 *    durante la validacion del sorteo.
 *  - operator<< declarado amigo para imprimir en una sola linea.
 * --------------------------------------------------------------
 */
class Confederacion {
private:
    std::string nombre;   // Nombre largo: "Union Europea de Futbol Asociacion"
    std::string codigo;   // Codigo corto: "UEFA", "CONMEBOL", "CONCACAF", etc.

public:
    // Constructor por defecto
    Confederacion();

    // Constructor con parametros
    Confederacion(const std::string& nombre, const std::string& codigo);

    // Getters
    std::string getNombre() const;
    std::string getCodigo() const;

    // Setters (para permitir cargar desde CSV sin recrear el objeto)
    void setNombre(const std::string& nombre);
    void setCodigo(const std::string& codigo);

    // Compara dos confederaciones por su codigo (que es el identificador).
    bool operator==(const Confederacion& otra) const;

    // Impresion como funcion amiga.
    friend std::ostream& operator<<(std::ostream& salida, const Confederacion& c);
};

#endif // CONFEDERACION_H
