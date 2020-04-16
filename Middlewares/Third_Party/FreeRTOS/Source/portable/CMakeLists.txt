target_sources(freertos
	PRIVATE
		GCC/ARM_CM3/port.c
		MemMang/heap_4.c
)

target_include_directories(freertos PUBLIC GCC/ARM_CM3)