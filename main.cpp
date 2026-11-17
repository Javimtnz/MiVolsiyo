#include <iostream>
#include <iomanip> // para usa setprecision
#include <nlohmann/json.hpp>
#include <fstream>
#include <optional>
#include <string>
#include <windows.h>

using namespace std;
using json = nlohmann::json;

// Comprobamos la clave y el tipo antes de convertir, en vez de dejar que
// nlohmann lance type_error: así el mensaje dice exactamente qué clave falla.
optional<double> leerPorcentaje(const json& datos, const string& clave) {
    if (!datos.contains(clave)) {
        cerr << "Error: falta la clave \"" << clave << "\" en regla-settings.json" << endl;
        return nullopt;
    }
    const json& valor = datos[clave];
    if (!valor.is_number()) {
        cerr << "Error: \"" << clave << "\" debe ser un número (ej. 0.50), no " << valor.dump() << endl;
        return nullopt;
    }
    double porcentaje = valor.get<double>();
    if (porcentaje < 0.0 || porcentaje > 1.0) {
        cerr << "Error: \"" << clave << "\" debe estar entre 0 y 1 (0.50 = 50%), no " << porcentaje << endl;
        return nullopt;
    }
    return porcentaje;
}

int main(){
     SetConsoleOutputCP(CP_UTF8);
     SetConsoleCP(CP_UTF8);

// 1. Abrir el archivo
    std::ifstream archivo("config/regla-settings.json");
    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir regla-settings.json" << std::endl;
        return 1;
    }
// 2. Parsear el contenido a un objeto json
    json datos;
    try {
        // json::parse (en vez de archivo >> datos) también falla si sobra texto tras el JSON
        datos = json::parse(archivo);
    } catch (const json::exception& e) {
        // json::exception engloba parse_error y también out_of_range,
        // que se lanza con números que no caben en double (ej. 1e400)
        std::cerr << "Error al leer regla-settings.json: " << e.what() << std::endl;
        return 1;
    }
    if (!datos.is_object()) {
        std::cerr << "Error: regla-settings.json debe ser un objeto { ... } con las claves Necesidades, Ocio y Ahorro" << std::endl;
        return 1;
    }

 // 3. Extraer los porcentajes del JSON
    optional<double> necesidades = leerPorcentaje(datos, "Necesidades");
    optional<double> ocio        = leerPorcentaje(datos, "Ocio");
    optional<double> ahorro      = leerPorcentaje(datos, "Ahorro");
    if (!necesidades || !ocio || !ahorro) {
        return 1;
    }

double salario;
cout << "Inserte salario (usa punto, ej. 1175.25): ";
cin >> salario;

     double A = salario * *necesidades;
     double B = salario * *ocio;
     double C = salario * *ahorro;

cout << fixed << setprecision(2); // para que los tres valores salgan siempre con 2 decimales
cout << "Así quedaría la regla: "
     << "Necesidades: " << A << "€" << endl
     << "Ocio: " << B << "€" << endl
     << "Ahorro e invesión: " << C << "€" << endl;

     return 0;
}
