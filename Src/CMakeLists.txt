target_sources(${elf_file}
	PRIVATE
		control.c
		input.c
		OvenMode.c
		sensor.c
		ProfileControl.cpp
		Reflow.cpp
)

add_subdirectory(Display)