	BITS 16
 	%INCLUDE "mikedev.inc"
	ORG 32768


os_command_line:
call os_clear_screen
call os_print_string
mov si, mynameis
call os_print_string
ret

mynameis 		db 13, 10, 0