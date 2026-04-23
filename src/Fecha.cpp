#include "Fecha.h"

// ------------------ Metodos auxiliares estaticos ------------------

bool Fecha::esBisiesto(int anio) {
    // Regla gregoriana: divisible por 4, excepto los multiplos de 100,
    // salvo que tambien sean multiplos de 400.
    if (anio % 400 == 0) return true;
    if (anio % 100 == 0) return false;
    return (anio % 4 == 0);
}

int Fecha::diasDelMes(int mes, int anio) {
    switch (mes) {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return esBisiesto(anio) ? 29 : 28;
        default:
            return 0; // mes invalido
    }
}

// Convierte a "serial" sumando los dias desde el anio 0.
// Es un calculo aproximado pero suficiente para diferencias
// dentro del mismo rango cercano (meses del torneo).
long Fecha::aSerial() const {
    long total = 0;
    // Sumar dias de anios completos previos
    for (int a = 1; a < anio; ++a) {
        total += esBisiesto(a) ? 366 : 365;
    }
    // Sumar dias de meses completos del anio actual
    for (int m = 1; m < mes; ++m) {
        total += diasDelMes(m, anio);
    }
    // Sumar los dias del mes actual
    total += dia;
    return total;
}

Fecha Fecha::desdeSerial(long serial) {
    // Recorremos anio a anio consumiendo dias.
    int a = 1;
    while (true) {
        int diasAnio = esBisiesto(a) ? 366 : 365;
        if (serial <= diasAnio) break;
        serial -= diasAnio;
        ++a;
    }
    int m = 1;
    while (true) {
        int diasMes = diasDelMes(m, a);
        if (serial <= diasMes) break;
        serial -= diasMes;
        ++m;
    }
    int d = static_cast<int>(serial);
    return Fecha(d, m, a);
}

// ------------------ Constructores ------------------

Fecha::Fecha() : dia(1), mes(1), anio(2026) {}

Fecha::Fecha(int dia, int mes, int anio)
    : dia(dia), mes(mes), anio(anio) {}

// ------------------ Getters ------------------

int Fecha::getDia()  const { return dia; }
int Fecha::getMes()  const { return mes; }
int Fecha::getAnio() const { return anio; }

// ------------------ Aritmetica ------------------

int Fecha::diferenciaDias(const Fecha& otra) const {
    return static_cast<int>(this->aSerial() - otra.aSerial());
}

Fecha Fecha::avanzar(int n) const {
    long serial = this->aSerial() + n;
    return desdeSerial(serial);
}

// ------------------ Comparaciones ------------------

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes  != otra.mes)  return mes  < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator<=(const Fecha& otra) const {
    return (*this < otra) || (*this == otra);
}

// ------------------ Impresion (funcion amiga) ------------------

std::ostream& operator<<(std::ostream& salida, const Fecha& f) {
    // Formato dd/mm/aaaa con ceros a la izquierda para dia y mes.
    if (f.dia < 10) salida << '0';
    salida << f.dia << '/';
    if (f.mes < 10) salida << '0';
    salida << f.mes << '/' << f.anio;
    return salida;
}
