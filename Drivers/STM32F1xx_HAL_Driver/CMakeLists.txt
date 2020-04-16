target_sources(stm32f1xx
	PRIVATE
		Src/stm32f1xx_hal_cortex.c
		Src/stm32f1xx_hal_dma.c
		Src/stm32f1xx_hal_exti.c
		Src/stm32f1xx_hal_flash_ex.c
		Src/stm32f1xx_hal_flash.c
		Src/stm32f1xx_hal_gpio_ex.c
		Src/stm32f1xx_hal_gpio.c
		Src/stm32f1xx_hal_i2c.c
		Src/stm32f1xx_hal_pwr.c
		Src/stm32f1xx_hal_rcc_ex.c
		Src/stm32f1xx_hal_rcc.c
		Src/stm32f1xx_hal_spi.c
		Src/stm32f1xx_hal_tim_ex.c
		Src/stm32f1xx_hal_tim.c
		Src/stm32f1xx_hal_wwdg.c
		Src/stm32f1xx_hal.c
)

target_include_directories(stm32f1xx PUBLIC Inc)