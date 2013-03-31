;************************************************
;
;	Subtracting weight amplifier and controller
;
;************************************************
;
; History:
;
;		27-04-2007 - Project started by CA. Preliminary command set defined.
;		17-05-2007 - Bootloader added.
;		18-05-2007 - Command set reduced to save code space and simplify use.
;		22-05-2007 - Restructering of the regulation loop. Now it actually works!
;
;************************************************
;
;	Command set:
;
;		B		Enter bootloader
;					Enters the bootloader allowing for firmware update on the fly.
;					The watchdog is NOT updated in the bootloader and the processor
;					will therefore be reset after 4 seconds when running the bootloader.
;					Only application space may be written by the bootloader. I.e. not the bootloader it self.
;					The bootloader is entered immediately and since the bootloader uses a different protocol
;					than the application no reply is sent.
;					Usage: B
;
;		E		EEPROM access
;					Reads and writes from/to the EEPROM.
;					The first parameter is the address in the EEPROM to access.
;					The second parameter is a '?' for read access
;					or a two digit hex number (the data to write) for write access.
;					Changes in the first 8 locations in the EEPROM only takes effect after a reboot or power cycle.
;					Usage: E<hh><hh> -> E<r><n>
;					       E<hh>? -> E<hh><n>
;
;		O		Reboot
;					Reboots the system by waiting for the watchdog timer to expire.
;					Usage: O -> O<n>
;
;		R		Get raw value
;					Get the raw value as read from the weight.
;					First parameter is the subtraction level.
;					Second parameter is the value read from the ADC.
;					Usage: R -> R<hh><hhhh><n>
;
;		S		Get stable-flag
;					Returns a flag indicating whether the weight is stable or fluctuating.
;					True means stable.
;					Usage: S -> S<b><n>
;
;		W		Get weight
;					Get the current weight using the calibration factors stored in EEPROM.
;					Usage: W -> W<hhhhhhhh><n>
;
;		L		Lock subtraction value
;					Locks the subtraction value to the specified value (used during calibration).
;					This stops the regular regulation loop.
;					Usage: L<hh> -> L<r><n>
;
;		U		Unlock subtraction value
;					Unlocks the subtraction value and starts the regulation loop.
;					Usage: U -> U<r><n>
;
;
; Legend:
;
; 	<hh>					A 2 digit hex number (i.e. an 8 bit value).
; 	<hhhh>				A 4 digit hex number (i.e. an 16 bit value).
; 	<hhhhhhhh>		An 8 digit hex number (i.e. an 32 bit value).
;		<b>						A flag/boolean. '0' means false. '1' means true.
; 	<n>						A new line character ('\n') with NO return character ('\r').
;		->						Indicates that the previous command results in the following reply.
;		<r>						Command reply. 'A' for acknowledge or 'N' for negative acknowledge.
;
;		Ordinary letters in the usage descriptions are ASCII letters
;		exactly as the are seen on the communication link.
;
;************************************************


;************************************************
; Device
;************************************************
.include "tn13def.inc"
;************************************************

; *** IO port definitions ***
.equ	RX_PIN										= PINB
.equ	RX_NR											= 0
.equ	TX_PORT										= PORTB
.equ	TX_NR											= 2

; *** Flags ***
.equ	Transmit									= 0
.equ	DataReady									= 1
.equ	Stable										= 2
;.equ	StableSignaled						= 3
.equ	ConversionStarted					= 4
.equ	TimerEvent								= 5
.equ	ConversionPhaseFlag				= 6
.equ	Locked										= 7


.equ	CONFIGURATION_BLOCK_SIZE	= 8


;************************************************
; SRAM variables
;************************************************
.dseg

.equ TX_RS232_BUFFER_SIZE			=	10
TxRS232Buffer:	.byte	TX_RS232_BUFFER_SIZE
TxRS232Head:		.byte	1
TxRS232Tail:		.byte	1

.equ RX_RS232_BUFFER_SIZE			=	10
RxRS232Buffer:	.byte	RX_RS232_BUFFER_SIZE
RxRS232Head:		.byte	1
RxRS232Tail:		.byte	1

/*
.equ FILTER_FIFO_SIZE					=	16
FilterFifo:			.byte	FILTER_FIFO_SIZE
FilterFifoHead:	.byte	1
*/

;************************************************
; STACK SPACE
;************************************************
.dseg

.equ	STACK_REQUIREMENT					= 20
STACK:					.byte	STACK_REQUIREMENT



;************************************************
; EEPROM data
;************************************************
.eseg

; Configuration section of the EEPROM - Only loaded at reboot/power cycle!
eeStableTolerance:			.db		32					; 
eeLowThreshold:					.dw		0+12				; Lower limit for the regulation loop.
eeHighThreshold:				.dw		1023-12			; Upper limit for the regulation loop.
eeMinimumSubtraction:		.db		1						; Lower limit for the subtraction value.
eeMaximumSubtraction:		.db		124					; Upper limit for the subtraction value.
eeReserved1:						.db		0						; Unused.
/*
eeSubtractionFactor:		.dd		65094656
eeWeightFactor:					.dd		519680
*/

;************************************************
; Register definitions
;************************************************
.def configStableTolerance			= r0			; RESERVED FOR CONFIGURATION SHADOW
.def configLowThresholdL				= r1			; RESERVED FOR CONFIGURATION SHADOW
.def configLowThresholdH				= r2			; RESERVED FOR CONFIGURATION SHADOW
.def configHighThresholdL				= r3 			; RESERVED FOR CONFIGURATION SHADOW
.def configHighThresholdH				= r4			; RESERVED FOR CONFIGURATION SHADOW
.def configMinimumSubtraction		= r5			; RESERVED FOR CONFIGURATION SHADOW
.def configMaximumSubtraction		= r6			; RESERVED FOR CONFIGURATION SHADOW
.def configReserved1						= r7			; RESERVED FOR CONFIGURATION SHADOW
.def Temp8											= r8
.def IntTemp										= r9
.def ADL												= r10
.def ADH												= r11
.def Temp7											= r12
.def Timer											= r13
.def RxRS232Count								= r14
.def TxRS232Data								= r15

.def Temp												= r16
.def Temp2											= r17
.def Temp3											= r18
.def Temp4											= r19
.def Flags											= r20
.def TxRS232State								= r21
.def RxRS232State								= r22
.def Temp5											= r23
.def Temp6											= r24
.def RxRS232Data								= r25
.def RS232Pointer								= r26 ; X
.def TxRS232Prescaler						= r27
.def IntPointer									= r28	; Y
.def RxRS232Prescaler						= r29
;.def nnnn												= r30	; Z
;.def nnnn												= r31


;************************************************
; Macros
;************************************************

.macro LED_ON
	sbi		PORTB, 3
.endmacro
.macro LED_OFF
	cbi		PORTB, 3
.endmacro
.macro LED_TOGGLE
	sbi		PINB, 3
.endmacro


;************************************************
; Interrupt Vectors
;************************************************
.cseg
.org 0
	rjmp	Reset

.org OVF0addr
	rjmp	Timer0Interrupt
;************************************************

Reset:

;	Setup ports
	ldi		Temp, 0x0e												; no pin
	out		DDRB, Temp
	ldi		Temp, 0x2c												; no pin
	out		PORTB, Temp

; Initialize SP
	ldi 	Temp, low(RAMEND)
	out 	SPL,Temp

; Setup watchdog
	wdr
	in		Temp, WDTCR
	ori		Temp, (1<<WDCE)|(1<<WDE)
	out		WDTCR, Temp
	ldi		Temp, (1<<WDE)|(1<<WDP2)					; Watchdog timeout of 250 mS
	out		WDTCR, Temp

; Setup Timer0
	ldi		Temp, 0x23												; Fast PWM on OCB
	out		TCCR0A, Temp
	ldi		Temp, 0x09												; ...and a prescaler of 1:1
	out		TCCR0B, Temp
	ldi		Temp, 125
	out		OCR0A, Temp												; A period of n clocks
	lsr		Temp															; Preset PWM to mid value
	out		OCR0B, Temp
	ldi		Temp, (1<<TOIE0)									; Interrupt on timer overflow (for the software UART and the time base)
	out		TIMSK0, Temp

; Setup ADC
	ldi		Temp, 0x02												; Vcc as the reference, right adjusted, (always) channel 2
	out		ADMUX, Temp
	ldi		Temp, 0x87												; Enable the ADC and set the prescaler to 1:128
	out		ADCSRA, Temp
	ldi		Temp, 0x10												; Disable the digital input buffer on the analog channel
	out		DIDR0, Temp

; Reset variables
	clr		Flags															; Clear all flags

	clr		TxRS232State											; Reset Tx state machine
	ldi		TxRS232Prescaler, 1

	clr		RxRS232State											; Reset Rx state machine
	clr		RxRS232Prescaler

	ldi		Temp, TxRS232Buffer								; Reset Tx FIFO
	sts		TxRS232Head, Temp
	sts		TxRS232Tail, Temp

	clr		RxRS232Count											; Reset Rx FIFO
	ldi		Temp, RxRS232Buffer
	sts		RxRS232Head, Temp
	sts		RxRS232Tail, Temp

; Enable global interrupt
	sei


BootloaderMainLoop_CheckForForcedReset:
	in		Temp, MCUSR
	sbrs	Temp, EXTRF
	rjmp	ApplicationStartingPoint
	andi	Temp, ~(1<<EXTRF)
	out		MCUSR, Temp


BootloaderMainLoop_WaitForStartChar:

	wdr

	tst		RxRS232Count
	breq	BootloaderMainLoop_WaitForStartChar

	rcall	GetC

Bootloader:

	sbi		PORTB, 3

	ldi		Temp, low(ApplicationStartingPoint); Send target info
	rcall	PutC
	ldi		Temp, high(ApplicationStartingPoint)
	rcall	PutC
	ldi		Temp, PAGESIZE
	rcall	PutC


BootloaderMainLoop:

	wdr

	rcall	GetC															; Get and store page number
	mov		Temp3, Temp
	clr		Temp4

	lsl		Temp3															; Format it to fit the SPM format
	rol		Temp4
	lsl		Temp3
	rol		Temp4
	lsl		Temp3
	rol		Temp4
	lsl		Temp3
	rol		Temp4
	lsl		Temp3
	rol		Temp4


BootloaderMainLoop_LoadPage:
	mov		ZL, Temp3
	mov		ZH, Temp4
	ldi		Temp2, PAGESIZE
BootloaderMainLoop_LoadPage_Loop:
	rcall	GetC
	mov		r0, Temp
	rcall	GetC
	mov		r1, Temp
	ldi		Temp, (1<<SPMEN)
	cli
	out		SPMCSR, Temp
	spm
	sei
	adiw	ZH:ZL, 2
	dec		Temp2
	brne	BootloaderMainLoop_LoadPage_Loop


BootloaderMainLoop_ErasePage:
	mov		ZL, Temp3
	mov		ZH, Temp4
	ldi		Temp, (1<<PGERS)|(1<<SPMEN)
	cli
	out		SPMCSR, Temp
	spm
	sei


BootloaderMainLoop_ProgramPage:
	mov		ZL, Temp3
	mov		ZH, Temp4
	ldi		Temp, (1<<PGWRT)|(1<<SPMEN)
	cli
	out		SPMCSR, Temp
	spm
	sei


BootloaderMainLoop_Acknowledge:

	ldi		Temp, 'A'
	rcall	PutC

	rjmp	BootloaderMainLoop

;************************************************************************************************

;************************************************************************************************


;************************************************
Timer0Interrupt:

	push	Temp
	in		Temp, SREG
	push	Temp


RxRS232:
	cpi		RxRS232State, 0
	breq	RxRS232_GetStartbit
	cpi		RxRS232State, 9
	breq	RxRS232_GetStopbit

RxRS232_GetDatabits:
	dec		RxRS232Prescaler
	brne	RxRS232_Done
	ldi		RxRS232Prescaler, 4
	clc
	sbic	RX_PIN, RX_NR
	sec
	ror		RxRS232Data
	inc		RxRS232State
	rjmp	RxRS232_Done

RxRS232_GetStartbit:
	sbic	RX_PIN, RX_NR
	rjmp	RxRS232_Done
RxRS232_GetStartbit_Detected:
	inc		RxRS232State
	ldi		RxRS232Prescaler, 6
	rjmp	RxRS232_Done

RxRS232_GetStopbit:
	dec		RxRS232Prescaler
	brne	RxRS232_Done
	sbis	RX_PIN, RX_NR
	rjmp	RxRS232_GetStopbit_Bad
RxRS232_GetStopbit_Good:

	lds		YL, RxRS232Head
	st		Y+, RxRS232Data
	inc		RxRS232Count
	cpi		YL, (RxRS232Buffer+RX_RS232_BUFFER_SIZE)
	brne	RxRS232_GetStopbit_Good_DontClear
	ldi		YL, RxRS232Buffer
RxRS232_GetStopbit_Good_DontClear:
	sts		RxRS232Head, YL
	sbr		Flags, (1<<DataReady)
	clr		RxRS232State
	clr		RxRS232Prescaler
	rjmp	RxRS232_Done
RxRS232_GetStopbit_Bad:
	clr		RxRS232State
	clr		RxRS232Prescaler

RxRS232_Done:


TxRS232:
	sbrs	Flags, Transmit
	rjmp	TxRS232_Done
	dec		TxRS232Prescaler
	brne	TxRS232_Done
	cpi		TxRS232State, 0
	breq	TxRS232_SendStartbit
	cpi		TxRS232State, 9
	breq	TxRS232_SendStopbit

TxRS232_SendDatabits:
	ror		TxRS232Data
	brcc	TxRS232_LowDatabit
	sbi		TX_PORT, TX_NR
TxRS232_LowDatabit:
	brcs	TxRS232_HighDatabit
	cbi		TX_PORT, TX_NR
TxRS232_HighDatabit:
	inc		TxRS232State
	rjmp	TxRS232_ResetPrescaler

TxRS232_SendStartbit:
	cbi		TX_PORT, TX_NR
	inc		TxRS232State
	lds		YL, TxRS232Tail
	ld		TxRS232Data, Y+																					; YL is same register as IntPointer
	cpi		YL, (TxRS232Buffer+TX_RS232_BUFFER_SIZE)
	brne	TxRS232_SendStartbit_DontClear
	ldi		YL, TxRS232Buffer
TxRS232_SendStartbit_DontClear:
	sts		TxRS232Tail, YL
	rjmp	TxRS232_ResetPrescaler

TxRS232_SendStopbit:
	sbi		TX_PORT, TX_NR
	clr		TxRS232State
	lds		IntTemp, TxRS232Head
	lds		Temp, TxRS232Tail
	cp		IntTemp, Temp
	brne	TxRS232_SendStopbit_Done
TxRS232_SendStopbit_BufferEmpty:
	cbr		Flags, (1<<Transmit)																; All done => stop sending
TxRS232_SendStopbit_Done:

TxRS232_ResetPrescaler:
	ldi		Temp, 4
	mov		TxRS232Prescaler, Temp

TxRS232_Done:


TimerBlock:
	dec		Timer
	brne	TimerBlock_Done
	sbr		Flags, (1<<TimerEvent)
TimerBlock_Done:


Timer0Compare_End:

	pop		Temp
	out		SREG, Temp
	pop		Temp

	reti
;************************************************


;************************************************
; Registers: Temp, XL
;************************************************
PutC:
	lds		RS232Pointer, TxRS232Head
	st		X+, Temp
	cpi		RS232Pointer, (TxRS232Buffer+TX_RS232_BUFFER_SIZE)
	brne	PutC_DontClear
	ldi		RS232Pointer, TxRS232Buffer
PutC_DontClear:
	sts		TxRS232Head, RS232Pointer
	sbr		Flags, (1<<Transmit)
	ret
;************************************************


;************************************************
; Registers: Temp, XL, Temp5
;************************************************
GetC:
	tst		RxRS232Count
	breq	GetC
	lds		RS232Pointer, RxRS232Tail
	ld		Temp, X+
	cpi		RS232Pointer, (RxRS232Buffer+RX_RS232_BUFFER_SIZE)
	brne	GetC_DontClear
	ldi		RS232Pointer, RxRS232Buffer
GetC_DontClear:
	sts		RxRS232Tail, RS232Pointer
	push	Temp
	lds		Temp, RxRS232Head
	cp		RS232Pointer, Temp
	pop		Temp
	brne	GetC_StillData
	cbr		Flags, (1<<DataReady)
GetC_StillData:
	dec		RxRS232Count
	ret
;************************************************


;************************************************************************************************
; END OF BOOTLOADER SPACE
;************************************************************************************************


























;************************************************************************************************
; ONLY CODE AFTER THIS POINT CAN BE UPDATED THROUGH THE BOOTLOADER!!!
;************************************************************************************************


.org 0x00e0
ApplicationStartingPoint:


; Load configuration
LoadConfiguration:
	clr		ZL
LoadConfiguration_Loop:
	mov		Temp, ZL
	rcall	EEPROMRead
	st		Z+, Temp
	cpi		ZL, CONFIGURATION_BLOCK_SIZE
	brne	LoadConfiguration_Loop

	
; Disable analog comparator
	sbi		ACSR, ACD

; Reset application variables
	clr		ADH
	clr		ADL

/*
; Init filter FIFO
	ldi		Temp, FilterFifo
	sts		FilterFifoHead, Temp
*/

;************************************************************************************************
;************************************************************************************************
MainLoop:

	wdr


CommandInterpreter:
;	tst		RxRS232Count
;	brne	CommandInterpreter_NewCommand
	sbrs	Flags, DataReady
	rjmp	CommandInterpreter_Done

CommandInterpreter_NewCommand:	
	rcall	GetC

;***************

CommandInterpreter_CommandEnterBootloader:
	cpi		Temp, 'B'
	brne	CommandInterpreter_CommandEnterBootloader_Miss

	rjmp	Bootloader
	
CommandInterpreter_CommandEnterBootloader_Miss:

;***************

CommandInterpreter_CommandEEPROM:
	cpi		Temp, 'E'
	brne	CommandInterpreter_CommandEEPROM_Miss

	rcall	PutC

	rcall	GetHex														; Get address
	brne	CommandInterpreter_CommandEEPROM_Error
	mov		ZL, Temp
	rcall	GetC
	cpi		Temp, '?'
	breq	CommandInterpreter_CommandEEPROM_ReadByte

CommandInterpreter_CommandEEPROM_WriteByte:
	rcall	GetHex_SkipFirstGetC							; Get data
	mov		Temp2,Temp
	mov		Temp, ZL
	rcall	EEPROMWrite
	ldi		Temp, 'A'
	rcall	PutC
	ldi		Temp, '\n'
	rcall	PutC
	rjmp	CommandInterpreter_Done

CommandInterpreter_CommandEEPROM_ReadByte:
	mov		Temp, ZL
	rcall	EEPROMRead
	rcall	PutHex
	ldi		Temp, '\n'
	rcall	PutC
	rjmp	CommandInterpreter_Done

CommandInterpreter_CommandEEPROM_Error:
	rjmp	CommandInterpreter_Unknown

CommandInterpreter_CommandEEPROM_Miss:

;***************

CommandInterpreter_CommandReboot:
	cpi		Temp, 'O'
	brne	CommandInterpreter_CommandReboot_Miss

	rcall	PutC
	ldi		Temp, '\n'
	rcall	PutC

CommandInterpreter_CommandReboot_DeadLoop:
	rjmp	CommandInterpreter_CommandReboot_DeadLoop
	
CommandInterpreter_CommandReboot_Miss:

;***************

CommandInterpreter_CommandRaw:
	cpi		Temp, 'R'
	brne	CommandInterpreter_CommandRaw_Miss

	rcall	PutC

	in		Temp, OCR0B
	rcall	PutHex
/*
CommandInterpreter_CommandRaw_AverageFifo:
	clr		Temp5
	clr		Temp
	lds		XL, FilterFifo
CommandInterpreter_CommandRaw_AverageFifoLoop:
	ld		Temp3, X+
	ld		Temp4, X+
	add		Temp5, Temp3
	adc		Temp, Temp4
	st		X+, ADL
	st		X+, ADH
	cpi		XL, FilterFifoHead
	brlo	CommandInterpreter_CommandRaw_AverageFifoLoop

	lsr		Temp
	ror		Temp5
	lsr		Temp
	ror		Temp5
	lsr		Temp
	ror		Temp5
	rcall	PutHex														; Send AD
	mov		Temp, Temp5
	rcall	PutHex
*/

	mov		Temp, ADH
	rcall	PutHex														; Send AD
	mov		Temp, ADL
	rcall	PutHex

	ldi		Temp, '\n'
	rcall	PutC

	rjmp	CommandInterpreter_Done
	
CommandInterpreter_CommandRaw_Miss:

;***************

CommandInterpreter_CommandStable:
	cpi		Temp, 'S'
	brne	CommandInterpreter_CommandStable_Miss
	
	rcall	PutC
	ldi		Temp, '0'
	sbrc	Flags, Stable
	ldi		Temp, '1'
	rcall	PutC
	ldi		Temp, '\n'
	rcall	PutC

	rjmp	CommandInterpreter_Done
	
CommandInterpreter_CommandStable_Miss:

;***************

CommandInterpreter_CommandGetWeight:
	cpi		Temp, 'W'
	brne	CommandInterpreter_CommandGetWeight_Miss

	rcall	PutC


	rjmp	CommandInterpreter_Done
	
CommandInterpreter_CommandGetWeight_Miss:

;***************

CommandInterpreter_CommandLock:
	cpi		Temp, 'L'
	brne	CommandInterpreter_CommandLock_Miss

	rcall	PutC

	sbr		Flags, (1<<Locked)

	rcall	GetHex
	brne	CommandInterpreter_CommandLock_InvalidValue

	out		OCR0B, Temp

	ldi		Temp, 'A'
	rjmp	CommandInterpreter_CommandLock_Reply

CommandInterpreter_CommandLock_InvalidValue:
	ldi		Temp, 'N'

CommandInterpreter_CommandLock_Reply:
	rcall	PutC
	ldi		Temp, '\n'
	rcall	PutC
	
	rjmp	CommandInterpreter_Done

CommandInterpreter_CommandLock_Miss:

;***************

CommandInterpreter_CommandUnlock:
	cpi		Temp, 'U'
	brne	CommandInterpreter_CommandUnlock_Miss

	cbr		Flags, (1<<Locked)

	rcall	PutC
	ldi		Temp, 'A'
	rcall	PutC
	ldi		Temp, '\n'
	rcall	PutC

	rjmp	CommandInterpreter_Done
	
CommandInterpreter_CommandUnlock_Miss:

;***************

CommandInterpreter_Unknown:
	ldi		Temp, '?'
	rcall	PutC
	ldi		Temp, '\n'
	rcall	PutC

;***************


CommandInterpreter_Done:





ConversionTask:

	sbrs	Flags, TimerEvent									; Has timeout period elapsed?
	rjmp	ConversionTask_Done								; No

	clr		Timer
	cbr		Flags, (1<<TimerEvent)

	sbrs	Flags, ConversionPhaseFlag
	rjmp	RegulationPhase


ConversionPhase:
	cbr		Flags, (1<<ConversionPhaseFlag)
	sbi		ADCSRA, ADSC	
	rjmp	ConversionTask_Done



RegulationPhase:
	sbr		Flags, (1<<ConversionPhaseFlag)

	in		Temp, ADCL
	in		Temp2, ADCH
	movw	ZH:ZL, ADH:ADL
	add		ADL, Temp
	adc		ADH, Temp2
	lsr		ADH
	ror		ADL

/*
FilterFifoUpdate:
	lds		XL, FilterFifoHead
	st		X+, ADL
	st		X+, ADH
	cpi		XL, FilterFifoHead
	brlo	FilterFifoUpdate_DontResetPoint
FilterFifoUpdate_ResetPoint:
	ldi		Temp, FilterFifo
	sts		FilterFifoHead, Temp
FilterFifoUpdate_DontResetPoint:
*/

;***** if(!locked)

	sbrc	Flags, Locked
	rjmp	ConversionTask_InRange_NotStable

;*****
; if(adc >= HIGH_THRESHOLD)
;   if(OCR0B < configMaximumSubtraction)
;     OCR0B++
;   Flags.Stable = false
ConversionTask_CheckHighLimit:
	cp		ADL, configHighThresholdL
	cpc		ADH, configHighThresholdH
	brlo	ConversionTask_CheckLowLimit

	in		Temp, OCR0B
	cp		Temp, configMaximumSubtraction
	brsh	ConversionTask_CheckHighLimit_DontChangeSubtraction
	inc		Temp
	out		OCR0B, Temp
ConversionTask_CheckHighLimit_DontChangeSubtraction:
	cbr		Flags, (1<<Stable)
	rjmp	ConversionTask_Done

;*****
; else if(adc <= LOW_THRESHOLD)
;   if(OCR0B > configMinimumSubtraction)
;     OCR0B--
;   Flags.Stable = false
ConversionTask_CheckLowLimit:
	cp		configLowThresholdL, ADL
	cpc		configLowThresholdH, ADH
	brlo	ConversionTask_InRange

	in		Temp, OCR0B
	cp		configMinimumSubtraction, Temp
	brsh	ConversionTask_CheckLowLimit_DontChangeSubtraction
	dec		Temp
	out		OCR0B, Temp
ConversionTask_CheckLowLimit_DontChangeSubtraction:
	cbr		Flags, (1<<Stable)
	rjmp	ConversionTask_Done

;*****
; else
;   if(tempNewADC < tempOldADC)
;     swap(tempNewADC, tempOldADC)
ConversionTask_InRange:

	movw	Temp4:Temp3, ADH:ADL
	movw	Temp2:Temp, ZH:ZL
	cp		Temp, Temp3
	cpc		Temp2, Temp4
	brsh	ConversionTask_InRange_DontSwap

ConversionTask_InRange_Swap:
	movw	Temp4:Temp3, ZH:ZL
	movw	Temp2:Temp, ADH:ADL
ConversionTask_InRange_DontSwap:


; if( (tempNewADC - tempOldADC) < configStableTolerance)
;   Flags.Stable = true
; else
;   Flags.Stable = false
ConversionTask_InRange_GetAbsoluteDifference:
	sub		Temp, Temp3
	sbc		Temp2, Temp4
	ldi		Temp3, 0
	cp		Temp, configStableTolerance
	cpc		Temp2, Temp3
	brsh	ConversionTask_InRange_NotStable

ConversionTask_InRange_Stable:
	sbr		Flags, (1<<Stable)
	rjmp	ConversionTask_Done

ConversionTask_InRange_NotStable:
	cbr		Flags, (1<<Stable)


;*****

ConversionTask_Done:


UpdateLED:
; copy stable flag to LED
	sbrs	Flags, Stable
	LED_OFF
	sbrc	Flags, Stable
	LED_ON
UpdateLED_Done:


	rjmp	MainLoop


;************************************************************************************************
;************************************************************************************************


;************************************************
; Registers: Temp, Temp2
;************************************************
PutHex:
	mov		Temp2, Temp
PutHex_High_ToAscii:
	swap	Temp
	andi	Temp, 0x0f
	cpi		Temp, 10
	brsh	PutHex_High_SameOrHigher
PutHex_High_Less:
	subi	Temp, -48
	rjmp	PutHex_High_PutC
PutHex_High_SameOrHigher:
	subi	Temp, -55
PutHex_High_PutC:
	rcall	PutC

PutHex_Low_ToAscii:
	mov		Temp, Temp2
	andi	Temp, 0x0f
	cpi		Temp, 10
	brsh	PutHex_Low_SameOrHigher
PutHex_Low_Less:
	subi	Temp, -48
	rjmp	PutHex_Low_PutC
PutHex_Low_SameOrHigher:
	subi	Temp, -55
PutHex_Low_PutC:
	rcall	PutC
	ret
;************************************************


;************************************************
; Registers: Temp, Temp2
;************************************************
GetHex:
	rcall	GetC
GetHex_SkipFirstGetC:

GetHex_ParseHighDigit:

GetHex_ParseHighDigit_VerifyNumeric:
	cpi		Temp, '0'
	brlo	GetHex_InvalidCharacter
	cpi		Temp, '9'+1
	brsh	GetHex_ParseHighDigit_VerifyAlpha
GetHex_ParseHighDigit_Numeric:
	subi	Temp, '0'
	mov		Temp2, Temp
	rjmp	GetHex_ParseLowDigit

GetHex_ParseHighDigit_VerifyAlpha:
	cpi		Temp, 'A'
	brlo	GetHex_InvalidCharacter
;	cpi		Temp, 'F'+1
;	brsh	GetHex_InvalidCharacter
GetHex_ParseHighDigit_Alpha:
	subi	Temp, 'A'-10
	mov		Temp2, Temp

GetHex_ParseLowDigit:
	rcall	GetC

GetHex_ParseLowDigit_VerifyNumeric:
	cpi		Temp, '0'
	brlo	GetHex_InvalidCharacter
	cpi		Temp, '9'+1
	brsh	GetHex_ParseLowDigit_VerifyAlpha
GetHex_ParseLowDigit_Numeric:
	subi	Temp, '0'
	rjmp	GetHex_MergeDigits

GetHex_ParseLowDigit_VerifyAlpha:
	cpi		Temp, 'A'
	brlo	GetHex_InvalidCharacter
;	cpi		Temp, 'F'+1
;	brsh	GetHex_InvalidCharacter
GetHex_ParseLowDigit_Alpha:
	subi	Temp, 'A'-10

GetHex_MergeDigits:
	swap	Temp2
	or		Temp, Temp2
	sez
	ret

GetHex_InvalidCharacter:
	clz
	ret
;************************************************


;************************************************
; Write Temp2 to address Temp
;************************************************
EEPROMWrite:
	push	Temp
	rcall	EEPROMRead
	cp		Temp, Temp2
	pop		Temp
	brne	EEPROMWrite_DoWrite
	ret
EEPROMWrite_DoWrite:
	out		EEARL, Temp
	out		EEDR, Temp2
	clr		Temp2
	out		EECR, Temp2
	cli
	sbi		EECR, EEMPE
	sbi		EECR, EEPE
	sei
	ret
;************************************************


;************************************************
; Read address Temp and put it in Temp
;************************************************
EEPROMRead:
	sbic	EECR, EEPE
 	rjmp	EEPROMRead
	out		EEARL, Temp
	sbi		EECR, EERE
	in		Temp, EEDR
	ret
;************************************************
