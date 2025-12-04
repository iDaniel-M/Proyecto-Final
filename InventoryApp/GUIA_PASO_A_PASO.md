# Guía Paso a Paso: Gestor de Inventario

Esta guía explica cómo configurar, compilar y entender el código fuente del "Gestor de Inventario para Hogar o Laboratorio".

## 1. Requisitos Previos

Para ejecutar este proyecto, necesitas instalar:

1.  **Compilador de C++** (GCC, Clang o MSVC).
2.  **Qt Framework** (versión 5.15 o 6.x).
    *   En Linux (Ubuntu/Debian): `sudo apt install qt6-base-dev libqt6sql6-sqlite`
    *   En Windows/macOS: Descargar el instalador desde [qt.io](https://www.qt.io/download-qt-installer). Asegúrate de seleccionar "Qt 6.x" y el componente "MinGW" (si estás en Windows y no tienes Visual Studio).
3.  **CMake** (versión 3.10 o superior).
4.  **Herramientas de construcción** (Make, Ninja, etc.).

## 2. Estructura del Proyecto

El código está organizado de la siguiente manera:

*   **`src/`**: Contiene los archivos de implementación (`.cpp`).
*   **`include/`**: Contiene los archivos de cabecera (`.h`).
*   **`CMakeLists.txt`**: Archivo de configuración para CMake.

### Clases Principales

1.  **`Component`**: Representa un artículo del inventario (nombre, tipo, cantidad, etc.).
2.  **`DatabaseManager`**: Maneja la conexión con la base de datos SQLite. Crea la tabla `components` si no existe y maneja las consultas SQL.
3.  **`InventoryManager`**: Es el intermediario entre la base de datos y la interfaz. Contiene la lógica de negocio, como buscar productos o filtrar stock bajo.
4.  **`ReportGenerator`**: Genera un archivo CSV con la lista actual de componentes.
5.  **`MainWindow`**: La ventana gráfica. Muestra la tabla de componentes y el formulario para editar/agregar.

## 3. Cómo Compilar y Ejecutar

### Opción A: Usando Qt Creator (Recomendado)

1.  Abre **Qt Creator**.
2.  Ve a `Archivo` > `Abrir archivo o proyecto...`.
3.  Selecciona el archivo `CMakeLists.txt` dentro de la carpeta `InventoryApp`.
4.  Configura el kit (selecciona tu versión de Qt instalada).
5.  Presiona el botón verde de "Play" (o `Ctrl+R`) para compilar y ejecutar.

### Opción B: Usando la Terminal (Linux/Mac/Git Bash)

Navega a la carpeta del proyecto y ejecuta:

```bash
cd InventoryApp
mkdir build
cd build
cmake ..
make
./InventoryApp
```

### Opción C: Si usas Windows y línea de comandos

Asegúrate de ejecutar esto en el "Developer Command Prompt for VS" o en una terminal con las variables de entorno de Qt configuradas:

```cmd
cd InventoryApp
mkdir build
cd build
cmake ..
cmake --build .
Debug\InventoryApp.exe
```

## 4. Funcionalidades del Sistema

*   **Agregar Componentes**: Llena el formulario a la derecha y presiona "Agregar".
*   **Ver Lista**: La tabla central muestra todos los componentes.
    *   **Alerta de Stock**: Si la cantidad es menor a 5, la fila se pondrá roja.
*   **Editar/Eliminar**: Selecciona una fila en la tabla. Los datos se cargarán en el formulario. Modifícalos y presiona "Actualizar", o presiona "Eliminar" para borrarlo.
*   **Buscar**: Escribe en el campo de búsqueda arriba para filtrar por nombre o tipo.
*   **Reportes**: Presiona "Generar Reporte CSV" para guardar un archivo compatible con Excel.

## 5. Detalles Técnicos

*   **Base de Datos**: Se usa SQLite. El archivo `inventory.db` se crea automáticamente en la carpeta de datos de usuario del sistema operativo (en Linux suele ser `~/.local/share/...`).
*   **Librerías Qt**:
    *   `QtWidgets`: Para la interfaz gráfica.
    *   `QtSql`: Para la base de datos.
    *   `QtPrintSupport`: Requerido por algunas dependencias de widgets.

## Notas Adicionales

Si estás trabajando en una **Raspberry Pi**:
1.  Instala las dependencias: `sudo apt install qt6-base-dev qt6-base-dev-tools libqt6sql6-sqlite build-essential cmake`.
2.  Sigue los pasos de compilación por terminal (Opción B).
