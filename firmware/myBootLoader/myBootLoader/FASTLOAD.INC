;*************************************************************************
;*									 *
;*				AVR universal Bootloader		 *
;*									 *
;*			Author: Peter Dannegger				 *
;*									 *
;*************************************************************************
.include "fastload.h"
.list
	.org	BootStart
init:
	cli				; no interrupts allowed
	ldi	a0, low (RamEnd)	; initialize stack
	out	SPL, a0
.ifdef SPH
	ldi	a0, high(RamEnd)
	out	SPH, a0
.endif
	clr	zerol			; for faster clear
	clr	zeroh

.if WDTRIGGER
.include "watchdog.inc"
.endif

	IOPortInit
.if CRC
	ldi	polynoml, 0x01
	ldi	polynomh, 0xA0
.endif
;-------------------------------------------------------------------------
.include "abaud.inc"			; measure baudrate
.include "password.inc"			; check password
;-------------------------------------------------------------------------
connected:
	ldi	a0, CONNECT		; password recognized
.if ONEWIRE
	rcall	syncputchar		; avoid message garbage
.else
	rcall	putchar
.endif
	rcall	getchar
	brne	connected		; until COMMAND received
;-------------------------------------------------------------------------
.include "command.inc"			; execute commands
.include "uart.inc"			; UART subroutines
;-------------------------------------------------------------------------
Password:
	.db	"Peda", 0, 0		; 'a' was recognized by ABAUD
.list
Messages:
	.db \
	3, high(Version), low(Version), \
	3, high(2*BufferSize), low(2*BufferSize), \
	4, SIGNATURE_000, SIGNATURE_001, SIGNATURE_002, \
	4, byte3(UserFlash), byte2(UserFlash), byte1(UserFlash)
;-------------------------------------------------------------------------
.if APICALL
.include "apicall.inc"			; program Flash from application
.else
	.org	Flashend
	ret
.endif
;-------------------------------------------------------------------------
