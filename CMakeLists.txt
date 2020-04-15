cmake_minimum_required(VERSION 3.13)

set(application_name "Rflw")

# set the build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE MATCHES Debug)
	message("Building for Debug.")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	message("Building for Release.")
endif()

# Project specific settings
project(${application_name} C CXX ASM)

message("Configuring ${application_name}")

# STM32 specific settings
set(DEVICE_FAMILY STM32F103xB)

set(LINKER_FILE ${PROJECT_SOURCE_DIR}/STM32F103RBTX_FLASH.ld)

# Add sub directory

# GCC optimization level: use -O0 in debug build, otherwise -O2
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(opt_level -O0)
else()
	set(opt_level -Os)
endif()

set(elf_file ${application_name}.elf)
set(bin_file ${application_name}.bin)
set(hex_file ${application_name}.hex)
set(map_file ${application_name}.map)
set(lss_file ${application_name}.lss)

# add sources to elf file
add_executable(${elf_file} "")

# link HAL library to project
add_subdirectory(Drivers)
target_link_libraries(${elf_file} PUBLIC stm32f1xx)

# link FreeRTOS to project
add_subdirectory(Middlewares/Third_Party/FreeRTOS/Source)
target_link_libraries(${elf_file} PUBLIC freertos)

# link source files to project
add_subdirectory(Core)
add_subdirectory(Src)

# include headers
target_include_directories(${elf_file} PUBLIC Inc)

# set additional for compiler and linker: optimization and generate map file
set(additional_compiler_flags ${opt_level})
set(additional_linker_flags -Wl,-Map=${map_file},--cref,--no-warn-mismatch)
target_compile_options(${elf_file} PRIVATE ${additional_compiler_flags})
target_link_libraries(${elf_file} PRIVATE ${additional_linker_flags})

# target_link_libraries(${elf_file} PUBLIC "-Wl, -mthumb")

# remove unused sections
target_link_libraries(${elf_file} PUBLIC "-g -Wl,--gc-sections")

# link with linker file
target_link_libraries(${elf_file} PUBLIC -T${LINKER_FILE})

# show size of resulting firmware image
add_custom_target(${elf_file}-size DEPENDS ${elf_file} COMMAND ${ARM_SIZE_EXECUTABLE} -B ${elf_file})

# generate extended listing
add_custom_target(${lss_file} DEPENDS ${elf_file} COMMAND ${ARM_OBJDUMP_EXECUTABLE} -S ${elf_file} > ${lss_file})

# create binary and hex files
add_custom_target(${hex_file} DEPENDS ${elf_file} COMMAND ${ARM_OBJCOPY_EXECUTABLE} -Oihex ${elf_file} ${hex_file})
add_custom_target(${bin_file} DEPENDS ${elf_file} COMMAND ${ARM_OBJCOPY_EXECUTABLE} -Obinary ${elf_file} ${bin_file})
add_custom_target(${application_name} ALL DEPENDS ${elf_file}-size ${bin_file} ${hex_file} ${lss_file})

# include stlink
include(CMake/stlink.cmake)