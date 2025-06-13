# Versão mínima do CMake necessária
cmake_minimum_required(VERSION 3.13)

# Configurações padrão para C/C++
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configuração automática para a extensão VS Code (não modificar)
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# Define a placa como Pico W (com Wi-Fi)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Nome do projeto
SET(Tarefa_FreeRTOS rp2040-freertos-template)

# Caminho para o FreeRTOS
SET(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)

# Importa o SDK do Pico e o kernel do FreeRTOS
include(pico_sdk_import.cmake)
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define o projeto
project(Tarefa_FreeRTOS C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Cria o executável principal
add_executable(Tarefa_FreeRTOS src/main.c)

# Configura nome e versão
pico_set_program_name(Tarefa_FreeRTOS "Tarefa_FreeRTOS")
pico_set_program_version(Tarefa_FreeRTOS "0.1")

# Desabilita stdio por UART/USB (não necessário para este projeto)
pico_enable_stdio_uart(Tarefa_FreeRTOS 0)
pico_enable_stdio_usb(Tarefa_FreeRTOS 0)

# Bibliotecas necessárias
target_link_libraries(Tarefa_FreeRTOS
    pico_stdlib                # Biblioteca padrão do Pico
    FreeRTOS-Kernel-Heap4      # FreeRTOS com gerenciador de memória Heap4
    hardware_pwm               # Para controle do buzzer via PWM
    hardware_i2c               # Caso precise usar I2C no futuro
)

# Configura diretórios de include
target_include_directories(Tarefa_FreeRTOS PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Gera arquivos adicionais (.hex, .bin etc)
pico_add_extra_outputs(Tarefa_FreeRTOS)