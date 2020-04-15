message("Using GNU ARM Toolchain")

# the name of the target operating system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE FILEPATH "Compiler")
set(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE FILEPATH "Compiler")
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc CACHE FILEPATH "Compiler")
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# core flags
set(COMPILER_OPTIMISATION "-g3")
set(CORE_FLAGS "-mthumb -mcpu=cortex-m3 --specs=nano.specs -mfloat-abi=soft ${ADDITIONAL_CORE_FLAGS}")

# compiler: language specific glags
# set(CMAKE_C_FLAGS "" CACHE INTERNAL "c compiler flags")
set(CMAKE_C_FLAGS "${CORE_FLAGS} ${COMPILER_OPTIMISATION} -std=gnu11 -ffunction-sections -fdata-sections -Wall -fstack-usage" CACHE INTERNAL "c compiler flags")
# set(CMAKE_C_FLAGS "${CORE_FLAGS} -fno-builtin -Wall -std=gnu99 -fdata-sections -ffunction-sections -g3 -gdwarf-2" CACHE INTERNAL "c compiler flags")
set(CMAKE_C_FLAGS_DEBUG "" CACHE INTERNAL "c compiler flags: Debug")
set(CMAKE_C_FLAGS_RELEASE "" CACHE INTERNAL "c compiler flags: Release")

set(CMAKE_CXX_FLAGS "${CORE_FLAGS} ${COMPILER_OPTIMISATION} -std=gnu++17 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE INTERNAL "cxx compiler flags: Debug")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE INTERNAL "cxx compiler flags: Release")

set(CMAKE_ASM_FLAGS "${CORE_FLAGS} ${COMPILER_OPTIMISATION} -c -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "" CACHE INTERNAL "asm compiler flags: Debug")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "asm compiler flags: Release")

# linker
# set(CMAKE_EXE_LINKER_FLAGS "${CORE_FLAGS} -Wl,-Map=linker.map -Wl,--gc-sections -static -Wl, --start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group" CACHE INTERNAL "exe link flags")
# SET(CMAKE_EXE_LINKER_FLAGS "${COMPILER_OPTIONS} -Wl,-Map=linker.map -Wl,-cref -Wl,--gc-sections" CACHE INTERNAL "exe link flags")
# set(CMAKE_EXE_LINKER_FLAGS "${CORE_FLAGS} -Wl, -Map=linker.map -Wl," CACHE INTERNAL "exe link flags")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_FILE}")

#-T"/home/julian/STM32CubeIDE/workspace/Reflow2/STM32F103RBTX_FLASH.ld" -Wl,-Map="${ProjName}.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment,
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

message(STATUS "System Processor      : ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS
    "BUILD FLAGS:\n"
    "   Core flags      : ${CORE_FLAGS}\n"
    "   c flags         : ${CMAKE_C_FLAGS}\n"
    "   c++ flags       : ${CMAKE_CXX_FLAGS}\n"
    "   asm flags       : ${CMAKE_ASM_FLAGS}\n"
	"   ld flags        : ${CMAKE_EXE_LINKER_FLAGS}\n"
	"   optimizations   : ${COMPILER_OPTIMISATION}\n"
)

find_program(ARM_SIZE_EXECUTABLE arm-none-eabi-size)
find_program(ARM_GDB_EXECUTABLE arm-none-eabi-gdb)
find_program(ARM_OBJCOPY_EXECUTABLE arm-none-eabi-objcopy)
find_program(ARM_OBJDUMP_EXECUTABLE arm-none-eabi-objdump)