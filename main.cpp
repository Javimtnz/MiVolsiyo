#include <iostream>
#include <iomanip> // para usa setprecision
#include "json.hpp"
#include <fstream>
#include <windows.h>

using namespace std;
using json = nlohmann::json;
     

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
        archivo >> datos;
    } catch (const json::parse_error& e) {
        std::cerr << "Error al parsear el JSON: " << e.what() << std::endl;
        return 1;
    }

 // 2. Extraer los porcentajes del JSON
    double necesidades = datos["Necesidades"];
    double ocio        = datos["Ocio"];
    double ahorro       = datos["Ahorro"];

double salario;
cout << "Inserte salario (usa punto, ej. 1175.25): ";
cin >> salario;

     double A = salario * necesidades;
     double B = salario * ocio;
     double C = salario * ahorro;

cout << fixed << setprecision(2); // para que los tres valores salgan siempre con 2 decimales
cout << "Así quedaría la regla: " 
     << "Necesidades: " << A << "€" << endl 
     << "Ocio: " << B << "€" << endl 
     << "Ahorro e invesión: " << C << "€" << endl;

     return 0;
}