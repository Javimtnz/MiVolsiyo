# MiVolsiyo

> Calculadora de la regla 50/30/20 — primer paso hacia una app de 
> contabilidad personal con almacenamiento local.

**Versión actual:** 0.0.0 (en desarrollo activo — sentando las bases)

## ¿Qué hace ahora mismo?

- Calcula la distribución 50/30/20 (necesidades / Ocio / ahorro) a partir del salario mensual.
- Se puede modificar la regla a tú gusto.
- Acepta el salario con punto o coma decimal (`1175.25` o `1175,25`) y vuelve a preguntar si el importe no es válido.
- Si la configuración tiene un error, explica cuál en vez de cerrarse sin más.
---
## Visión del proyecto

Este proyecto está pensado para crecer hasta convertirse en una herramienta 
de contabilidad personal que permita:

- [ ] Almacenar el salario mensual localmente
- [ ] Registrar carteras de inversión
- [ ] Llevar seguimiento de activos y pasivos

Ahora mismo el foco está en construir una base sólida (arquitectura, 
modelo de datos, lógica de cálculo) antes de añadir estas funciones.

---
## Estado del desarrollo

Proyecto en fase temprana y cambia con frecuencia. La estructura de 
datos y la lógica interna pueden romperse entre versiones hasta 
llegar a 1.0.0.

En el [CHANGELOG](CHANGELOG.md) puede ver los cambios más detallados entre versiones.

---
## Instalación

>[!warning] **Solo windows.** El programa usa `windows.h`, por lo que no compila en Linux ni en macOS. 

### 1. Instalar el compilador

1. Instala [MSYS2](https://www.msys2.org/) con su instalador.
2. En el menú de inicio de Windows busca **MSYS2 UCRT64**.
3. instala g++:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

4. Comprueba que lo tienes instalado.

```bash 
g++ --version 
```

### 2. Descarga el proyecto

```bash
git clone https://github.com/Javimtnz/MiVolsiyo.git 
cd MiVolsiyo
```

Si no usas Git, pulsa **Code → Download ZIP** en GitHub, descomprime y entra en la carpeta desde el terminal.

### 3. Descargar librería

MiVolsiyo usa [nlohmann/json](https://github.com/nlohmann/json), que se puede descargar de dos formas:

**Opción A:**

Descargando el [`json.hpp`](https://github.com/nlohmann/json/releases/latest/download/json.hpp) y guardarlo en una carpeta **`nlohmann/` dentro de la carpeta del proyecto** (`MiVolsiyo/nlohmann/json.hpp`), que si vas a usar la librería en un solo proyecto o para que funcione este programa, es la más limpia. La carpeta `nlohmann/` hace falta porque el código lo incluye como `<nlohmann/json.hpp>`.

**Opción B:**

- abrir el terminal **MSYS2 UCRT64** y ejecutar:

```bash
pacman -S mingw-w64-ucrt-x86_64-nlohmann-json
```

- Comprobar la instalación:
```bash
pacman -Q mingw-w64-ucrt-x86_64-nlohmann-json
```

- No hace falta copiar ningún archivo al proyecto: el compilador la encuentra solo, porque `pacman` la deja en sus rutas del sistema. En el código se incluye así:

```cpp
#include <nlohmann/json.hpp>
```

### 4. Compilar

Desde la carpeta del proyecto:

```bash
g++ -std=c++17 main.cpp -o main.exe
```

Si usaste la **opción A**, añade `-I.` para que el compilador busque también en la carpeta del proyecto:

```bash
g++ -std=c++17 -I. main.cpp -o main.exe
```

### 5. Ejecutar

```bash
./main.exe
```

El programa busca `config/regla-settings.json` junto a `main.exe`, así que funciona aunque lo lances desde otra carpeta. Si mueves `main.exe`, lleva la carpeta `config/` con él.

>## Problemas frecuentes

| Error                    | Causa probable                                         |    
| ------------------------ | ------------------------------------------------------ | 
| `g++: command not found` | No estás en el terminal **UCRT64**, o falta el paso 1. |
| `nlohmann/json.hpp: No such file or directory` | Falta el paso 3: con la opción A, revisa que esté en `nlohmann/json.hpp` y que compilas con `-I.`. |
| `Error: no se pudo abrir ...\config\regla-settings.json` | La carpeta `config/` no está junto a `main.exe`. El mensaje muestra la ruta donde la ha buscado. |
| `Error: falta la clave "..."` / `debe ser un número` / `debe estar entre 0 y 1` | Hay un error en `config/regla-settings.json`; el mensaje dice qué clave corregir. |




---
## Decisiones de diseño
### Configuración en `.json` excepto en el código

**Decisión:** Los porcentajes de la regla 50/30/20 se leen desde un archivo `.json` al arrancar, en lugar de estar formando parte del código directamente.

**Motivos:**
- Cambiar cualquier cosa en C++ implica recompilar de nuevo el código, en cambio, con el `.json` cualquier persona puede entrar al archivo y modificarlo sin necesidad de compilar de nuevo.
- Da cierto nivel de personalización para que los usuarios puedan adaptar la regla a sus necesidades. 
- Separa lo que es la lógica del programa con parámetros de configuración que pueden variar.
- Me da la base para poder ir añadiendo ajustes en un futuro.
- `nlohmann/json` me permite leer de manera sencilla un `.json` y no hace falta instalación, solo descargar el header.

**Contrapartidas:**
- Un error en el `.json` y deja de funcionar el programa.
- Archivo extra que suma al set que se ha de distribuir.
- Tienes que tener una dependencia externa o sino no te funcionará el programa. 

### Dinero en céntimos enteros en lugar de decimales

**Decisión:** Los importes se guardan y se calculan en céntimos con números enteros (`1175,25 €` → `117525`) y solo se convierten a euros para mostrarlos.

**Motivos:**
- MiVolsiyo va a ser una app de finanzas personales: cualquier fallo en el cálculo, aunque sea de unos céntimos, obliga a mover cantidades ínfimas para cuadrar las cuentas, y eso hace perder el tiempo.

**Contrapartidas:**
- Hay que convertir el importe al leerlo y al mostrarlo.
- Al repartir por porcentajes todavía se puede descuadrar 1 céntimo, porque cada parte se redondea por separado. Está planeado que la última parte se lleve el resto para que el total cuadre siempre.

### Configuración junto al ejecutable

**Decisión:** El programa busca `config/regla-settings.json` en la misma carpeta que `main.exe`, en vez de en la carpeta desde la que se lanza.

**Motivos:**
- Así es fácil de descargar y usar: basta con tener `main.exe` y `config/` juntos, se lance como se lance.
- En fase de desarrollo, antes de la beta, es la opción más rápida; no me planteo cambiarla por ahora.

**Contrapartidas:**
- Si se mueve `main.exe`, hay que llevar la carpeta `config/` con él.
- Depende de otra función de Windows (`GetModuleFileNameW`) para saber dónde está el ejecutable.

---
## Integraciones

| Include          | Tipo                                                                     | Notas                                                                                  |
| ---------------- | ------------------------------------------------------------------------ | -------------------------------------------------------------------------------------- |
| `<windows.h>`    | SDK de Windows (siempre presente en Windows)                             | Windows-only. Pone la consola en UTF-8 y localiza la carpeta de `main.exe` (`GetModuleFileNameW`) |
| `<nlohmann/json.hpp>` | Librería de terceros ([nlohmann/json](https://github.com/nlohmann/json)) | Header-only; no incluida en el repo, hay que descargarla manualmente antes de compilar |

---

Este proyecto se desarrolla con ayuda de [Claude Code](https://claude.com/claude-code).
