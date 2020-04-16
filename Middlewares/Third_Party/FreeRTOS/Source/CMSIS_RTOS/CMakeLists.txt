target_sources(freertos
	PRIVATE
		cmsis_os.c
)

target_include_directories(freertos PUBLIC .)