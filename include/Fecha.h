#ifndef FECHA_H
#define FECHA_H

#include <iostream>

/*
 * Clase Fecha
 * --------------------------------------------------------------
 * Representa una fecha calendario (dia, mes, anio) y provee la
 * aritmetica necesaria para la asignacion de partidos del torneo:
 *   - diferencia en dias entre dos fechas
 *   - avanzar N dias
 *   - comparaciones (==, <)
 *   - impresion con operator<< (funcion amiga)
 *
 * Decisiones de diseño:
 *  - No maneja memoria dinamica, por lo que NO requiere Regla
 *    de los Tres (constructor/destructor/operator= sinteticos
 *    del compilador son suficientes).
 *  - Se usa dia juliano simplificado para calcular diferencias:
 *    se convierte la fecha a un numero serial y se opera sobre el.
 *  - operator<< se declara amiga pues ostream no es miembro.
 * --------------------------------------------------------------
 */
class Fecha {
private:
    int dia;
    int mes;
    int anio;

    // Indica si el anio dado es bisiesto (regla gregoriana).
    static bool esBisiesto(int anio);

    // Devuelve cuantos dias tiene el mes dado en el anio dado.
    static int diasDelMes(int mes, int anio);

    // Convierte la fecha a un numero serial de dia (dia juliano
    // proleptico simplificado). Permite restar fechas facilmente.
    long aSerial() const;

    // Construye una fecha a partir de un serial (inversa de aSerial).
    static Fecha desdeSerial(long serial);

public:
    // Constructor por defecto: 01/01/2026 (inicio razonable para el torneo).
    Fecha();

    // Constructor con parametros.
    Fecha(int dia, int mes, int anio);

    // Getters
    int getDia() const;
    int getMes() const;
    int getAnio() const;

    // Diferencia en dias con respecto a otra fecha (this - otra).
    // Positivo si this es posterior, negativo si es anterior.
    int diferenciaDias(const Fecha& otra) const;

    // Devuelve una nueva fecha resultado de avanzar n dias desde this.
    // Si n es negativo, retrocede.
    Fecha avanzar(int n) const;

    // Comparaciones
    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;

    // Impresion formato dd/mm/aaaa como funcion amiga (requisito del diagrama).
    friend std::ostream& operator<<(std::ostream& salida, const Fecha& f);
};

#endif // FECHA_H
