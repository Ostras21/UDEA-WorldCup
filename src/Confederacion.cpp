#include "Confederacion.h"

Confederacion::Confederacion() : nombre(""), codigo("") {}

Confederacion::Confederacion(const std::string& nombre, const std::string& codigo)
    : nombre(nombre), codigo(codigo) {}

std::string Confederacion::getNombre() const { return nombre; }
std::string Confederacion::getCodigo() const { return codigo; }

void Confederacion::setNombre(const std::string& nombre) { this->nombre = nombre; }
void Confederacion::setCodigo(const std::string& codigo) { this->codigo = codigo; }

bool Confederacion::operator==(const Confederacion& otra) const {
    // Dos confederaciones son iguales si comparten el mismo codigo.
    return codigo == otra.codigo;
}

std::ostream& operator<<(std::ostream& salida, const Confederacion& c) {
    salida << c.codigo;
    // Si hay nombre extendido disponible, lo imprime entre parentesis.
    if (!c.nombre.empty()) {
        salida << " (" << c.nombre << ")";
    }
    return salida;
}
