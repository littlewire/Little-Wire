;------------------------------------------------------------------------
;			redefinitions for compatibility
;------------------------------------------------------------------------
.ifndef	WDTCSR
.equ	WDTCSR	= WDTCR
.endif
;---------------------------
.ifndef	WDCE
.equ	WDCE	= WDTOE
.endif
;---------------------------
.ifndef	SPMCSR
.equ    SPMCSR	= SPMCR
.endif
;---------------------------
.ifndef	RWWSRE
.ifdef  ASRE
.equ    RWWSRE  = ASRE
.endif
.endif
;---------------------------
.ifndef SPMEN
.equ    SPMEN	= SELFPRGEN
.endif
;----------------------	macros for extended IO access -------------------
.macro	xout
.if	@0 > 0x3F
	sts	@0, @1
.else
	out	@0, @1
.endif
.endmacro
;---------------------------
.macro	xin
.if	@1 > 0x3F
	lds	@0, @1
.else
	in	@0, @1
.endif
.endmacro
;---------------------------
.macro  xlpm
.if FLASHEND > 0x7FFF
	elpm	@0, @1
.else
	lpm	@0, @1
.endif
.endmacro
;------------------------------------------------------------------------
