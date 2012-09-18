;-------------------------------------------------------------------------
;		 		Program User Flash
;-------------------------------------------------------------------------
;
Program:
	ldi	xl, low (ProgBuff)
	ldi	xh, high(ProgBuff)
	movw	yh:yl, xh:xl
	brts	_pro5
;---------------------- Receive data for one Page ------------------------
_pro1:
	rcall	getchar
	brne	_pro2
	rcall	getchar
	subi	a0, ESC_SHIFT
	set				; end mark received set
	breq	_pro3
	clt
_pro2:
	st	x+, a0
	cpi	xl, low(ProgBuffEnd)	; since buffer size below 256
	brne	_pro1
;-------------------------------------------------------------------------
_pro3:
	adiw	zh:zl, 0
	brne	_pro5
;------------------------------ Insert rjmp to boot loader ---------------
	ld	appl, y
	ldd	apph, y+1
	subi	appl, low (BootStart - 0x1001)	; new application jump
	sbci	apph, high(BootStart - 0x1001)
	ldi	a0, low (BootStart-1)
	ldi	a1, high(BootStart-1 + 0xC000)  ; = RJMP
	st	y, a0
	std	y+1, a1				; replace by bootloader jump
;-------------------------------------------------------------------------
;               Erase application Flash backward (avoid lock out)
;-------------------------------------------------------------------------
	ldi     zl, low (2*BootStart)
	ldi     zh, high(2*BootStart)
_pro4:
.if PageSize < 32
	sbiw	zh:zl, 2*PageSize
.else
	subi    zl, low (2*PageSize)
	sbci    zh, high(2*PageSize)
.endif
	ldi	a0, 1<<PGERS^1<<SPMEN
	out     SPMCSR, a0
	SPM                             ; CPU halted until erase done
	brne    _pro4			; until Z = 0x0000
;-------------------------------------------------------------------------
_pro5:
	brtc	_pro6
	std	y+2*PageSize-2, appl
	std	y+2*PageSize-1, apph
;---------------------- Fill page buffer ---------------------------------
_pro6:
	ld	r0, y+
	ld	r1, y+
	ldi	a0, 1<<SPMEN		; fill buffer command
	out	SPMCSR, a0
	SPM
	adiw	zh:zl, 2
	cpi	yl, low(ProgBuffEnd)
	brne	_pro6
.if PageSize < 32
	sbiw	zh:zl, 2*PageSize
.else
	subi	zl, low (PageSize * 2)
	sbci	zh, high(PageSize * 2)
.endif
;---------------------- Program page -------------------------------------
	ldi	a0, 1<<PGWRT^1<<SPMEN	; write page command
	out	SPMCSR, a0
	SPM
;---------------------- Next Page ----------------------------------------
.if PageSize < 32
	adiw	zh:zl, 2*PageSize
.else
	subi	zl, low (-PageSize * 2)
	sbci	zh, high(-PageSize * 2)	; point to next page
.endif
	brts	_pro8
	ldi	a0, CONTINUE
	rcall	putchar
_pro8:
	cpi	zl, low( 2*BootStart)
	ldi	a0, high(2*BootStart)
	cpc	zh, a0                  ; last page reached ?
	brcs	Program
	brts	_pro9
	rjmp	main_error		; error, size exceeded
_pro9:
	rjmp	main_ok
;-------------------------------------------------------------------------
