#ifndef CONFEDERACION_H
#define CONFEDERACION_H

#include <string>
#include <iostream>


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
