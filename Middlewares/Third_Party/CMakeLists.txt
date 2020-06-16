add_library(freertos
	FreeRTOS/croutine.c
	FreeRTOS/event_groups.c
	FreeRTOS/list.c
	FreeRTOS/queue.c
	FreeRTOS/stream_buffer.c
	FreeRTOS/tasks.c
	FreeRTOS/timers.c
	FreeRTOS/portable/GCC/ARM_CM3/port.c
	FreeRTOS/portable/MemMang/heap_4.c
)

target_include_directories(freertos PUBLIC FreeRTOS/include)
target_include_directories(freertos PUBLIC FreeRTOS/portable/GCC/ARM_CM3)
# Include FreeRTOSConfig.h
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/Core/Inc)
target_include_directories(freertos PUBLIC ${PROJECT_SOURCE_DIR}/Drivers/CMSIS/Include)
