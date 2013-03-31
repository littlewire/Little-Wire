;-------------------------------------------------------------------------
;		 		Program User Flash
;-------------------------------------------------------------------------
_pro1:
	ldi	a0, CONTINUE
	rcall	putchar
program:
	set
	ldi	xl, low (ProgBuff)
	ldi	xh, high(ProgBuff)
	ldi	yh, high(ProgBuffEnd)
;------------------------------ Receive data into buffer -----------------
_pro2:
	rcall	getchar
	brne	_pro3
	rcall	getchar
	subi	a0, ESC_SHIFT
	brne	_pro3			; A5,80 = end mark
	brts	_pro6
	set
	rjmp	_pro4
_pro3:
	clt
	st	x+, a0
	cpi	xl, low (ProgBuffEnd)
	cpc	xh, yh
	brne	_pro2
;-------------------------------------------------------------------------
_pro4:
	ldi	xl, low (ProgBuff)
	ldi	xh, high(ProgBuff)
_pro5:
	rcall	prog_page		; CY = 1: o.k
	brcc	main_error		; error, bootloader reached
	subi	zl, low (-2*PageSize)
	sbci	zh, high(-2*PageSize)	; point to next page
.if FLASHEND > 0x7FFF
	sbci    zx, byte3(-2*BufferSize)
.endif
	cpi	xl, low (ProgBuffEnd)
	cpc	xh, yh
	brne	_pro5			; until buffer end
	brtc	_pro1
;-------------------------------------------------------------------------
_pro6:
	rjmp	main_ok
;-------------------------------------------------------------------------
;			Program page in Flash
;-------------------------------------------------------------------------
;use:	r0, r1, a0, xl, xh, zl, zh
;
;input:  X  = buffer to RAM
;        Z  = page to program
;output: CY = 0: error, attempt to overwrite itself
;
.equ    PAGEMASK	= (PAGESIZE * 2 - 1) & ~1

prog_page:
;------------------------------ Avoid self destruction ! -----------------
	cpi	zl, low (2*BootStart)
	ldi	a0, high(2*BootStart)
	cpc	zh, a0                  	; below bootloader ?
.if FLASHEND > 0x7FFF
	ldi     a0, byte3(2*BootStart)
	cpc     zx, a0
.endif
	brcc	_prp3				; CY = 0: error
;------------------------------ Fill page buffer -------------------------
_prp1:	ld	r0, x+
	ld	r1, x+
	ldi	a0, 1<<SPMEN
	rcall	do_spm
	adiw	zh:zl, 2
	mov	a0, zl
	andi	a0, low(PAGEMASK)
	brne	_prp1
;------------------------------ Erase page -------------------------------
	subi	zl, low (2*PAGESIZE)
	sbci	zh, high(2*PAGESIZE)
	ldi	a0, 1<<PGERS^1<<SPMEN		; erase page command
	rcall	do_spm
;------------------------------ Program page -----------------------------
	ldi	a0, 1<<PGWRT^1<<SPMEN		; write page command
	rcall	do_spm
	ldi	a0, 1<<RWWSRE^1<<SPMEN
do_spm:
	xout	SPMCSR, a0
.if FLASHEND > 0x7FFF
	out     RAMPZ, zx               ; 3 byte Z pointer
	xout    SPMCSR, a0
.endif
	spm
_prp2:
	xin	a0, SPMCSR
	sbrc	a0, SPMEN
	rjmp	_prp2
	sec					; CY = 1: successful
_prp3:
	ret
;-------------------------------------------------------------------------
