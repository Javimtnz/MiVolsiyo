Todos los cambios relevantes de MiVolsiyo se documentan en este archivo. El formato se basa en [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/) y el proyecto usa [Versionado Semántico](https://semver.org/lang/es/).

## [Sin publicar] — Fecha

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