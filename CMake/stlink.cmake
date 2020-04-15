# ST_Link functions
# Adds targets for ST-Link programmers and emulators

# Configure flasher script for the project
set(FLASH_START 0x08000000)

#Add JLink commands
add_custom_target(debug 
	COMMAND ${DEBUGGER} -tui -command ${CMAKE_CURRENT_LIST_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${elf_file}
)

add_custom_target(debug-server 
	COMMAND st-util --listen_port 2331
	DEPENDS ${elf_file}
)

add_custom_target(flash 
	COMMAND st-flash --reset write ${bin_file} ${FLASH_START}
	DEPENDS ${elf_file}
)

add_custom_target(erase 
	COMMAND st-flash erase
)