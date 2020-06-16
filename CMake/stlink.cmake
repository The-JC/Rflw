# ST_Link functions
# Adds targets for ST-Link programmers and emulators

# Configure flasher script for the project
set(FLASH_START 0x08000000)
set(OPENOCD_CONFIG ${PROJECT_SOURCE_DIR}/tools/stm32.cfg)
set(OPENOCD_SCRIPTS /usr/share/openocd/scripts)

#Add JLink commands
add_custom_target(debug 
	COMMAND ${DEBUGGER} -tui -command ${CMAKE_CURRENT_LIST_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${elf_file}
)

add_custom_target(debug-server 
	COMMAND openocd -f ${OPENOCD_CONFIG} -s ${OPENOCD_SCRIPTS}
	DEPENDS ${elf_file}
)

add_custom_target(flash 
	COMMAND openocd -f ${OPENOCD_CONFIG} --command 
		"program ${elf_file} reset exit"
	DEPENDS ${elf_file}
)

add_custom_target(erase 
	COMMAND st-flash erase
)