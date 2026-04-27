#ifndef LISTA_H
#define LISTA_H

#include <stdexcept>


template <class T>
class Lista {
private:
    T* datos;          // arreglo dinamico de elementos
    int tamano;        // cantidad actual de elementos
    int capacidad;     // capacidad reservada en memoria

    // Redimensiona el arreglo interno a una nueva capacidad.
    // Si nuevaCapacidad <= capacidad actual, no hace nada.
    void redimensionar(int nuevaCapacidad) {
        if (nuevaCapacidad <= capacidad) return;
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < tamano; ++i) {
            nuevo[i] = datos[i];
        }
        delete[] datos;
        datos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    // ---------- Constructores / Destructor (Regla de los Tres) ----------

    // Constructor por defecto: capacidad inicial minima.
    Lista() : datos(nullptr), tamano(0), capacidad(0) {
        capacidad = 4;
        datos = new T[capacidad];
    }

    // Constructor de copia: realiza copia profunda.
    Lista(const Lista<T>& otra) : datos(nullptr), tamano(otra.tamano), capacidad(otra.capacidad) {
        datos = new T[capacidad];
        for (int i = 0; i < tamano; ++i) {
            datos[i] = otra.datos[i];
        }
    }

    // Destructor: libera el arreglo dinamico.
    ~Lista() {
        delete[] datos;
        datos = nullptr;
    }

    // Operador de asignacion: copia profunda con proteccion de auto-asignacion.
    Lista<T>& operator=(const Lista<T>& otra) {
        if (this == &otra) return *this;
        delete[] datos;
        tamano = otra.tamano;
        capacidad = otra.capacidad;
        datos = new T[capacidad];
        for (int i = 0; i < tamano; ++i) {
            datos[i] = otra.datos[i];
        }
        return *this;
    }

    // ---------- Operaciones principales ----------

    // Agrega un elemento al final. Duplica capacidad si es necesario.
    void agregar(const T& elemento) {
        if (tamano >= capacidad) {
            redimensionar(capacidad * 2);
        }
        datos[tamano++] = elemento;
    }

    // Elimina el elemento en la posicion indicada desplazando los siguientes.
    void eliminar(int indice) {
        if (indice < 0 || indice >= tamano) {
            throw std::out_of_range("Lista::eliminar: indice fuera de rango");
        }
        for (int i = indice; i < tamano - 1; ++i) {
            datos[i] = datos[i + 1];
        }
        --tamano;
    }

    // Acceso por indice (lectura / escritura).
    T& operator[](int indice) {
        if (indice < 0 || indice >= tamano) {
            throw std::out_of_range("Lista::operator[]: indice fuera de rango");
        }
        return datos[indice];
    }

    // Acceso por indice (solo lectura, para objetos const).
    const T& operator[](int indice) const {
        if (indice < 0 || indice >= tamano) {
            throw std::out_of_range("Lista::operator[]: indice fuera de rango");
        }
        return datos[indice];
    }

    // ---------- Consultas ----------

    int getTamano() const { return tamano; }
    int getCapacidad() const { return capacidad; }
    bool vacia() const { return tamano == 0; }

    // Vacia la lista manteniendo la capacidad reservada.
    void limpiar() { tamano = 0; }
};

#endif // LISTA_H
