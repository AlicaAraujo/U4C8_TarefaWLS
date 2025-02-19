# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Set the Pico board type
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(TarefaWLS_U4C8 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable with your source files
add_executable(TarefaWLS_U4C8
    TarefaWLS_U4C8.c
    bibliotecas/botao.c
    bibliotecas/leds.c
    bibliotecas/pwm.c
    bibliotecas/ssd1306.c
    bibliotecas/joystick.c
)

# Set the program name and version
pico_set_program_name(TarefaWLS_U4C8 "TarefaWLS_U4C8")
pico_set_program_version(TarefaWLS_U4C8 "0.1")

# Enable/disable UART/USB output
pico_enable_stdio_uart(TarefaWLS_U4C8 1)
pico_enable_stdio_usb(TarefaWLS_U4C8 1)

# Link libraries required by your program
target_link_libraries(TarefaWLS_U4C8
    pico_stdlib
    hardware_pwm
    hardware_i2c
    hardware_adc
    hardware_pio
    pico_bootrom
    hardware_clocks
    hardware_uart
)

# Set include directories for the project
target_include_directories(TarefaWLS_U4C8 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add extra outputs for the project (e.g., UF2, bin, etc.)
pico_add_extra_outputs(TarefaWLS_U4C8)
