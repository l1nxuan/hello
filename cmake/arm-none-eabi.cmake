set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

find_program(CMAKE_ASM_COMPILER arm-none-eabi-gcc     REQUIRED)
find_program(CMAKE_C_COMPILER   arm-none-eabi-gcc     REQUIRED)
find_program(CMAKE_LINKER       arm-none-eabi-g++     REQUIRED)
find_program(CMAKE_OBJCOPY      arm-none-eabi-objcopy REQUIRED)
find_program(CMAKE_SIZE         arm-none-eabi-size    REQUIRED)

if(DEFINED ENV{ASMFLAGS})
    set(CMAKE_ASM_FLAGS_DEBUG "$ENV{ASM_FLAGS_DEBUG}")
endif()

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")

# prevent CMake from trying to create executables during try_compile calls
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
