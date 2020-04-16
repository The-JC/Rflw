add_library(stm32f1xx "")

add_subdirectory(STM32F1xx_HAL_Driver)
add_subdirectory(CMSIS)

target_include_directories(stm32f1xx PUBLIC ${PROJECT_SOURCE_DIR}/Core/Inc)

# set(DRIVERS 
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c
# 	STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c)

# add_library(stm32f1xx ${DRIVERS})

# # add include directories for HAL libary and CMSIS
# target_include_directories(stm32f1xx PUBLIC STM32F1xx_HAL_Driver/Inc)
# target_include_directories(stm32f1xx PUBLIC CMSIS/Include)

# # set up some defines for the StdPeriph library
# target_compile_definitions(stm32f1xx PUBLIC USE_STDPERIPH_DRIVER)
target_compile_definitions(stm32f1xx PUBLIC ${DEVICE_FAMILY})

# # additional compiler options: use size-optimized version of library in release build, use -O0 in debug build
# if(CMAKE_BUILD_TYPE MATCHES Debug)
#   set(additional_flags -O0)
# else()
#   set(additional_flags -Os)
# endif()

target_compile_options(stm32f1xx PRIVATE ${additional_flags})