add_library(freertos
	croutine.c
	event_groups.c
	list.c
	queue.c
	stream_buffer.c
	tasks.c
	timers.c
)

target_include_directories(freertos PUBLIC include)
# Include FreeRTOSConfig.h
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/Core/Inc)
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include)

add_subdirectory(portable)
add_subdirectory(CMSIS_RTOS)