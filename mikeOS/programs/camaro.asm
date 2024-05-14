;this is for testing
	BITS 16
	%INCLUDE "mikedev.inc"
	ORG 32768
main:
	mov si, hello
	call os_print_string
	ret
hello : db "Hi How are you!!", 13, 10 , 0
