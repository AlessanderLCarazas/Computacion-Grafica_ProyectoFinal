# CG Proyecto Final: Proyecto de Reconocimiento y Superposición de Manos en 3D 

## Descripción del Proyecto
Este laboratorio tiene como objetivo superponer modelos 3D de manos en imágenes y videos reales utilizando técnicas avanzadas de visión por computadora. Se basa en la calibración precisa de cámaras y el uso de patrones de tablero de ajedrez para garantizar la correcta alineación del modelo 3D en el entorno visual.

### Principales Funcionalidades
1. **Carga de modelos 3D:** Importación de archivos STL para procesar geometrías tridimensionales.
   ![image](https://github.com/user-attachments/assets/0ac642df-d938-4081-9e48-388779636cf8)

3. **Calibración de cámara:** Uso de imágenes de tableros de ajedrez para calcular parámetros intrínsecos y extrínsecos de la cámara.
4. **Proyección de modelos:** Superposición precisa de geometrías 3D sobre imágenes capturadas.

## Tecnologías Utilizadas
- **OpenCV:** Para el procesamiento de imágenes y la calibración de cámaras.
- **Assimp:** Para la carga y gestión de modelos 3D en diversos formatos.
- **CMake:** Sistema de compilación utilizado para organizar el proyecto.
- **C++:** Lenguaje principal de desarrollo.

## Estructura del Código
### Archivos Principales
- `main.cpp`: Contiene la lógica principal del programa.
- `original.cpp`: Versión alternativa para experimentación con funcionalidades similares.

### Funciones Clave
- **`loadSTL`:** Carga modelos 3D desde archivos STL utilizando Assimp.
- **`projectSTLObject`:** Proyecta puntos 3D en imágenes 2D basándose en la calibración de la cámara.
- **`cv::calibrateCamera`:** Realiza la calibración de la cámara a partir de puntos 2D y 3D.

## Proceso de Calibración
1. Captura imágenes de un tablero de ajedrez en diferentes ángulos.
2. Identifica los esquinas del tablero en las imágenes usando `cv::findChessboardCorners`.
3. Calcula parámetros de calibración como:
   - Matriz de la cámara (`cameraMatrix`).
   - Coeficientes de distorsión (`distCoeffs`).

## Requisitos del Sistema
- **Dependencias:**
  - OpenCV
  - Assimp
  - CMake
- **Entorno de Desarrollo:**
  - Compilador compatible con C++11 o superior.
  - Sistema operativo Linux o Windows.

![image](https://github.com/user-attachments/assets/46d7460d-87bf-447a-9626-a13f92195e46)
![image](https://github.com/user-attachments/assets/b77d9fd5-951b-45d2-bbf2-fe810863b7b6)
![image](https://github.com/user-attachments/assets/682eeef6-704b-4423-a988-b9ae3d8a3bba)
![image](https://github.com/user-attachments/assets/ce356d56-acdc-4c3b-9fc6-c4e4f5348fc3)


