target_sources(${elf_file}
	PRIVATE
		Startup/startup_stm32f103rbtx.s
)

target_include_directories(${elf_file} PUBLIC Inc)

add_subdirectory(Src)