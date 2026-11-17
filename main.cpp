#include <iostream>
#include <iomanip> // para setw y setfill
#include <nlohmann/json.hpp>
#include <fstream>
#include <optional>
#include <string>
#include <sstream>
#include <cmath>   // para llround
#include <filesystem>
#include <windows.h>

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

// Carpeta donde está main.exe. Una ruta relativa como "config/..." se resuelve
// desde la carpeta en la que se lanza el programa, no desde la del .exe;
// partiendo de aquí, config/ se encuentra se ejecute desde donde se ejecute.
fs::path carpetaDelEjecutable() {
    wchar_t ruta[MAX_PATH];
    DWORD longitud = GetModuleFileNameW(nullptr, ruta, MAX_PATH);
    if (longitud == 0 || longitud == MAX_PATH) {
        return fs::current_path(); // falló o la ruta no cabía: volvemos al comportamiento antiguo
    }
    return fs::path(ruta).parent_path();
}

// El 100 % expresado en puntos básicos (1 punto básico = 0,01 %).
const long long CIEN_POR_CIEN = 10000;

// Comprobamos la clave y el tipo antes de convertir, en vez de dejar que
// nlohmann lance type_error: así el mensaje dice exactamente qué clave falla.
// Devuelve el porcentaje en puntos básicos (0.50 -> 5000) para poder sumar y
// repartir con enteros exactos: sumando double, 0.6 + 0.3 + 0.1 no da 1.
optional<long long> leerPorcentaje(const json& datos, const string& clave) {
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
    double enPuntosBasicos = porcentaje * CIEN_POR_CIEN;
    long long puntosBasicos = llround(enPuntosBasicos);
    // El margen absorbe el error de representación del double (0.29 * 10000 = 2899.9999...);
    // por encima de él es que el porcentaje tiene más decimales de los admitidos.
    if (fabs(enPuntosBasicos - puntosBasicos) > 1e-6) {
        cerr << "Error: \"" << clave << "\" admite como mucho 4 decimales (0.1234 = 12,34%), no " << valor.dump() << endl;
        return nullopt;
    }
    return puntosBasicos;
}

// Convierte "1175.25" o "1175,25" en 117525 céntimos leyendo los dígitos uno a uno,
// sin pasar por double: así el importe es exacto. Si el texto no es un importe
// válido (letras, signo, más de 2 decimales...) devuelve nullopt.
optional<long long> textoACentimos(const string& texto) {
    size_t inicio = texto.find_first_not_of(" \t\r");
    if (inicio == string::npos) return nullopt; // línea vacía
    size_t fin = texto.find_last_not_of(" \t\r");

    long long euros = 0;
    long long centimos = 0;
    int decimales = -1; // -1 = todavía no ha aparecido el punto o la coma
    bool hayDigitos = false;

    for (size_t i = inicio; i <= fin; ++i) {
        char c = texto[i];
        if (c == '.' || c == ',') {
            if (decimales != -1) return nullopt; // dos separadores, ej. "1.175,25"
            decimales = 0;
        } else if (c >= '0' && c <= '9') {
            hayDigitos = true;
            if (decimales == -1) {
                if (euros > 1000000000000LL) return nullopt; // evita desbordar long long
                euros = euros * 10 + (c - '0');
            } else {
                if (decimales == 2) return nullopt; // no existen fracciones de céntimo
                centimos = centimos * 10 + (c - '0');
                ++decimales;
            }
        } else {
            return nullopt;
        }
    }
    if (!hayDigitos) return nullopt;
    if (decimales == 1) centimos *= 10; // "12.5" son 50 céntimos, no 5
    return euros * 100 + centimos;
}

// Pregunta hasta recibir un importe válido mayor que 0.
// Devuelve nullopt si se acaba la entrada (Ctrl+Z o entrada redirigida vacía),
// para no quedarse preguntando en bucle para siempre.
optional<long long> pedirSalarioEnCentimos() {
    string linea;
    while (true) {
        cout << "Inserte salario (ej. 1175.25 o 1175,25): ";
        if (!getline(cin, linea)) return nullopt;
        optional<long long> centimos = textoACentimos(linea);
        if (centimos && *centimos > 0) return centimos;
        cout << "Importe no válido: usa solo números, hasta 2 decimales y mayor que 0." << endl;
    }
}

// 117525 -> "1175.25"
string formatearEuros(long long centimos) {
    ostringstream salida;
    salida << centimos / 100 << '.' << setw(2) << setfill('0') << centimos % 100;
    return salida.str();
}

int main(){
     SetConsoleOutputCP(CP_UTF8);
     SetConsoleCP(CP_UTF8);

// 1. Abrir el archivo
    fs::path rutaConfig = carpetaDelEjecutable() / "config" / "regla-settings.json";
    std::ifstream archivo(rutaConfig);
    if (!archivo.is_open()) {
        // u8string() en vez de string(): la ruta puede tener tildes y la consola está en UTF-8
        std::cerr << "Error: no se pudo abrir " << rutaConfig.u8string() << std::endl;
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
    optional<long long> necesidades = leerPorcentaje(datos, "Necesidades");
    optional<long long> ocio        = leerPorcentaje(datos, "Ocio");
    optional<long long> ahorro      = leerPorcentaje(datos, "Ahorro");
    if (!necesidades || !ocio || !ahorro) {
        return 1;
    }
    long long suma = *necesidades + *ocio + *ahorro;
    if (suma != CIEN_POR_CIEN) {
        cerr << "Error: los porcentajes de regla-settings.json suman "
             << suma / 100 << '.' << setw(2) << setfill('0') << suma % 100
             << "% y deben sumar 100%" << endl;
        return 1;
    }

// 4. Pedir el salario, ya en céntimos enteros
    optional<long long> salario = pedirSalarioEnCentimos();
    if (!salario) {
        cerr << endl << "Error: no se recibió ningún salario." << endl;
        return 1;
    }

// 5. Repartir. El porcentaje es decimal, así que se redondea una sola vez
//    al céntimo más cercano y a partir de ahí todo son enteros exactos.
    long long A = llround(*salario * (*necesidades / double(CIEN_POR_CIEN)));
    long long B = llround(*salario * (*ocio / double(CIEN_POR_CIEN)));
    long long C = llround(*salario * (*ahorro / double(CIEN_POR_CIEN)));

cout << "Así quedaría la regla: "
     << "Necesidades: " << formatearEuros(A) << "€" << endl
     << "Ocio: " << formatearEuros(B) << "€" << endl
     << "Ahorro e inversión: " << formatearEuros(C) << "€" << endl;

     return 0;
}
