#include "MedidorRecursos.h"

MedidorRecursos::MedidorRecursos() : iteraciones(0), memoriaBytes(0) {}

// ---------- Iteraciones ----------
void MedidorRecursos::registrarIteracion()       { ++iteraciones; }
void MedidorRecursos::registrarIteraciones(long n) { iteraciones += n; }
long MedidorRecursos::getIteraciones() const     { return iteraciones; }
void MedidorRecursos::resetIteraciones()         { iteraciones = 0; }

// ---------- Memoria ----------
void MedidorRecursos::registrarMemoria(long bytes) { memoriaBytes += bytes; }
void MedidorRecursos::liberarMemoria(long bytes) {
    memoriaBytes -= bytes;
    if (memoriaBytes < 0) memoriaBytes = 0; // proteccion ante calculos imprecisos
}
long MedidorRecursos::getMemoriaBytes() const { return memoriaBytes; }

// ---------- Reporte ----------
void MedidorRecursos::reportar(const std::string& funcionalidad) const {
    // Conversion legible: bytes / KB / MB segun la magnitud.
    double valor = static_cast<double>(memoriaBytes);
    std::string unidad = "bytes";
    if (memoriaBytes >= 1024L * 1024L) {
        valor = memoriaBytes / (1024.0 * 1024.0);
        unidad = "MB";
    } else if (memoriaBytes >= 1024L) {
        valor = memoriaBytes / 1024.0;
        unidad = "KB";
    }

    std::cout << "\n+----------------------------------------------------+\n";
    std::cout <<   "| MEDIDOR DE RECURSOS                                |\n";
    std::cout <<   "+----------------------------------------------------+\n";
    std::cout <<   "| Funcionalidad : " << funcionalidad << "\n";
    std::cout <<   "| Iteraciones   : " << iteraciones << "\n";
    std::cout <<   "| Memoria activa: " << valor << " " << unidad
              <<                       " (" << memoriaBytes << " bytes)\n";
    std::cout <<   "+----------------------------------------------------+\n";
}
