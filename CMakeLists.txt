
cmake_minimum_required(VERSION 3.5)
project(metavision_sdk_started)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(MetavisionSDK_DIR "/usr/share/cmake/MetavisionSDK")

# Encontre os componentes do SDK:
find_package(MetavisionSDK COMPONENTS base driver ui REQUIRED)

# Encontrar os módulos necessários do Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Diretórios de include adicionais (HAL manual)
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Nome do executável:
set(EXECUTABLE_NAME eventCam)

# Indica os arquivos fontes:
set(SOURCES
    src/main.cpp
    src/eventoViewer.cpp
    src/menuGUI.cpp 

    #include/funcoesEventos.hpp
)

# Adicione o executável:
add_executable(eventCam ${SOURCES})

# Vincule corretamente as bibliotecas do SDK
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE
        MetavisionSDK::base
        MetavisionSDK::driver
        MetavisionSDK::ui
        Qt5::Widgets  
)