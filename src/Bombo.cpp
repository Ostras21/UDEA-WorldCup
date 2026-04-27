#include "Bombo.h"
#include <random>

// RNG estatico compartido por todas las instancias de Bombo.
// Se siembra una sola vez con std::random_device para garantizar
// sorteos no reproducibles (a diferencia del simulador de partido,
// donde podria interesar una seed fija para depurar).
static std::mt19937& rngBombo() {
    static std::mt19937 generador(std::random_device{}());
    return generador;
}

Bombo::Bombo() : numero(0), equipos() {}
Bombo::Bombo(int numero) : numero(numero), equipos() {}

// ---------- Getters ----------
int Bombo::getNumero() const          { return numero; }
int Bombo::cantidad()  const          { return equipos.getTamano(); }
Lista<Equipo*>&       Bombo::getEquipos()       { return equipos; }
const Lista<Equipo*>& Bombo::getEquipos() const { return equipos; }

// ---------- Setters ----------
void Bombo::setNumero(int n) { numero = n; }

// ---------- Operaciones ----------
void Bombo::agregar(Equipo* e) {
    if (e != nullptr) equipos.agregar(e);
}

Equipo* Bombo::extraerAleatorio() {
    int n = equipos.getTamano();
    if (n == 0) return nullptr;
    std::uniform_int_distribution<int> dist(0, n - 1);
    int indice = dist(rngBombo());
    Equipo* elegido = equipos[indice];
    equipos.eliminar(indice);
    return elegido;
}

void Bombo::devolver(Equipo* e) {
    if (e != nullptr) equipos.agregar(e);
}

bool Bombo::vacio() const {
    return equipos.getTamano() == 0;
}

std::ostream& operator<<(std::ostream& salida, const Bombo& b) {
    salida << "Bombo " << b.numero
           << " [" << b.equipos.getTamano() << " equipos]: ";
    for (int i = 0; i < b.equipos.getTamano(); ++i) {
        if (b.equipos[i] != nullptr) {
            salida << b.equipos[i]->getCodigo();
            if (i + 1 < b.equipos.getTamano()) salida << ", ";
        }
    }
    return salida;
}
