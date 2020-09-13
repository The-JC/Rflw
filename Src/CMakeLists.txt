target_sources(${elf_file}
	PRIVATE
		control.c
		input.c
		OvenMode.c
		sensor.c
		ProfileControl.c
		Reflow.c
)

add_subdirectory(Display)