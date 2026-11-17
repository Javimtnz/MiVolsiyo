Todos los cambios relevantes de MiVolsiyo se documentan en este archivo. El formato se basa en [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/) y el proyecto usa [Versionado Semántico](https://semver.org/lang/es/).

## [Sin publicar] — Fecha

### Cambiado

- El salario acepta punto o coma decimal (`1175.25` o `1175,25`) y vuelve a preguntar si el importe no es válido.
- Los importes se calculan en céntimos enteros en lugar de `double`, para que el dinero sea exacto.
- `config/regla-settings.json` se busca junto al ejecutable, así que el programa funciona aunque se lance desde otra carpeta.

### Corregido

- Una clave ausente o con texto en lugar de número en `regla-settings.json` cerraba el programa con una excepción sin capturar; ahora se explica qué clave falla. También se rechazan porcentajes fuera de 0–1, números que desbordan (`1e400`), texto sobrante tras el JSON y un JSON que no sea un objeto.
- Escribir letras o una coma en el salario hacía que se calculara con 0 o con el número truncado sin avisar.
- Errata "invesión" en la salida.

### Planeado

- Validar que los porcentajes de la regla sumen 100 antes de continuar.
- Almacenar el salario mensual localmente.
- Registrar carteras de inversión.
- Llevar seguimiento de activos y pasivos.
- Implementar un GUI mínimo e intuitivo.

## [0.0.0] — 15-09-2026

### Añadido

- Cálculo de la distribución 50/30/20 (necesidades / ocio / ahorro) a partir del salario mensual.
- Configuración de los porcentajes mediante un archivo `settings.json` en `config/`.

### Interno

- Dependencia de `nlohmann/json` para leer la configuración.
- README con secciones de instalación, decisiones de diseño e integraciones.