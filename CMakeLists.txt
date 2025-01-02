#[===[
cmake_minimum_required(VERSION 3.10)
project(3DModelProjection)

find_package(OpenCV REQUIRED)
find_package(VTK REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${VTK_INCLUDE_DIRS})

add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS} ${VTK_LIBRARIES})
]===]


cmake_minimum_required(VERSION 3.10)
project(3DModelProjection)

# Establecer la política para preferir GLVND
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

# Encontrar OpenCV
find_package(OpenCV REQUIRED)

# Encontrar VTK
find_package(VTK REQUIRED)

# Encontrar OpenGL
find_package(OpenGL REQUIRED)

# Encontrar GLEW
find_package(GLEW REQUIRED)

find_package(Assimp REQUIRED)

# Encontrar GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

# Incluir directorios de cabeceras
include_directories(${OpenCV_INCLUDE_DIRS} ${VTK_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

# Crear el ejecutable
add_executable(main main.cpp)

# Vincular bibliotecas necesarias
target_link_libraries(main ${OpenCV_LIBS} ${VTK_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW3_LIBRARIES} ${ASSIMP_LIBRARIES})
