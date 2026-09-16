# [MiVolsiyo]

> Calculadora de la regla 50/30/20 — primer paso hacia una app de 
> contabilidad personal con almacenamiento local.

**Versión actual:** 0.0.0 (en desarrollo activo — sentando las bases)

## ¿Qué hace ahora mismo?

- Calcula la distribución 50/30/20 (necesidades / Ocio / ahorro) a partir del salario mensual.

## Visión del proyecto

Este proyecto está pensado para crecer hasta convertirse en una herramienta 
de contabilidad personal que permita:

- [ ] Almacenar el salario mensual localmente
- [ ] Registrar carteras de inversión
- [ ] Llevar seguimiento de activos y pasivos

Ahora mismo el foco está en construir una base sólida (arquitectura, 
modelo de datos, lógica de cálculo) antes de añadir estas funciones.

## Estado del desarrollo

Proyecto en fase temprana y cambia con frecuencia. La estructura de 
datos y la lógica interna pueden romperse entre versiones hasta 
llegar a 1.0.0.

## Instalación
...

## Integraciones


|  Include   |  Tipo   | Notas |
| --- | --- | --- |
|  `<windows.h>`   |  SDK de Windows (siempre presente en Windows)   | Windows-only |
|  `json.hpp`   |  Librería de terceros ([nlohmann/json](https://github.com/nlohmann/json))  | Header-only; no incluida en el repo, hay que descargarla manualmente antes de compilar |