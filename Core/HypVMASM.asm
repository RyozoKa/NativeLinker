_TEXT SEGMENT

EXTERNDEF opcodes : DQ
EXTERN ?findClass@HClass@@SAPEAV1@_K@Z : PROC
EXTERN ??4HString@@QEAAAEAV0@AEBV0@@Z: PROC			; HString::operator= &string
EXTERN ??4HString@@QEAAAEAV0@_J@Z: PROC				; = int64
EXTERN ??4HString@@QEAAAEAV0@_K@Z: PROC				; = uint64
EXTERN ??4HString@@QEAAAEAV0@N@Z: PROC				; = double
EXTERN ?append@HString@@QEAAAEAV1@PEB_WH@Z: PROC ;String.append(wchar_t*, int len)
EXTERN RegisterFunction PROC

PUBLIC ?CallVirtualFunction@HypThread@@QEAAXPEAVHObject@@G@Z
PUBLIC PUSHC
PUBLIC PUSHG
PUBLIC PUSHGN
PUBLIC PUSHGS
PUBLIC PUSHL
PUBLIC PUSHLI
PUBLIC PUSHSLI
PUBLIC PUSHLP
PUBLIC PUSHGP
PUBLIC PUSHGNP

PUBLIC MV2RBX
PUBLIC MV2R14
PUBLIC MV2RDX
PUBLIC MV2R8
PUBLIC MV2R9
PUBLIC MV2RSI
PUBLIC MV2RDI

PUBLIC MV2XMM1
PUBLIC MV2XMM2
PUBLIC MV2XMM3
PUBLIC MV2XMM4
PUBLIC MV2XMM5

PUBLIC MVRBX
PUBLIC MVR14
PUBLIC MVRDX
PUBLIC MVR8
PUBLIC MVR9
PUBLIC MVRSI
PUBLIC MVRDI

PUBLIC MVXMM1
PUBLIC MVXMM2
PUBLIC MVXMM3
PUBLIC MVXMM4
PUBLIC MVXMM5

PUBLIC S1P
PUBLIC S2P
PUBLIC S3P
PUBLIC S4P
PUBLIC S5P
PUBLIC S6P

PUBLIC R1P
PUBLIC R2P
PUBLIC R3P
PUBLIC R4P
PUBLIC R5P
PUBLIC R6P

PUBLIC SD1P
PUBLIC SD2P
PUBLIC SD3P
PUBLIC SD4P
PUBLIC SD5P
PUBLIC SD6P

PUBLIC RD1P
PUBLIC RD2P
PUBLIC RD3P
PUBLIC RD4P
PUBLIC RD5P
PUBLIC RD6P

PUBLIC CALLNAT
PUBLIC CALLNAT_T
PUBLIC CALLVIRT
PUBLIC CALLVIRT_T
PUBLIC CALLSTATIC
PUBLIC CALLSTATIC_T
PUBLIC CALLSTATNAT
PUBLIC CALLSTATNAT_T

PUBLIC LOADCLASS

	?CallVirtualFunction@HypThread@@QEAAXPEAVHObject@@G@Z PROC
		push r15	;save instruction pointer
		push r13	;save this pointer
		push r12	;save instructions
		push r11	;Need this!
		push r10
		push rbp	;save previous stack frame
		mov rbp, rsp	;set new stackframe
		mov	rax, QWORD PTR [rdx + 24]		
		mov r15, QWORD PTR [rax + r8*8h]
		mov r13, rdx	;Set this pointer
		mov r10, QWORD PTR [r13 + 8h]
		mov r12, [opcodes]
		sub sp, WORD PTR [r15+18h]		;Allocate stack space for locals
		mov r15, QWORD PTR [r15 + 8h]

		sub rsp, 128
		mov XMMWORD PTR[rsp + 112]	, xmm0
		mov XMMWORD PTR[rsp + 96]	, xmm1
		mov XMMWORD PTR[rsp + 80]	, xmm2	;We have to save all these registers
		mov XMMWORD PTR[rsp + 64]	, xmm3
		mov XMMWORD PTR[rsp + 48]	, xmm4
		mov XMMWORD PTR[rsp + 32]	, xmm5
		mov XMMWORD PTR[rsp + 16]	, xmm6
		mov XMMWORD PTR[rsp]		, xmm7

		test BYTE PTR [r15], 255	;AND test
		jz SHORT @ends	;If function returns right away, jump to the end
		@executes:
		mov r11, WORD PTR[r15]
		call QWORD PTR[r12 + (r11*8h)]
		test BYTE PTR[r15], 255
		jnz SHORT @executes
		@ends:

		mov xmm0,	XMMWORD PTR[rsp + 112]	
		mov xmm1,	XMMWORD PTR[rsp + 96]	
		mov xmm2,	XMMWORD PTR[rsp + 80]	
		mov xmm3,	XMMWORD PTR[rsp + 64]	
		mov xmm4,	XMMWORD PTR[rsp + 48]	
		mov xmm5,	XMMWORD PTR[rsp + 32]	
		mov xmm6,	XMMWORD PTR[rsp + 16]	
		mov xmm7,	XMMWORD PTR[rsp]		

		mov rsp, rbp
		pop rbp
		pop r10
		pop r11
		pop r12
		pop r13
		pop r15
		ret
	?CallVirtualFunction@HypThread@@QEAAXPEAVHObject@@G@Z ENDP

	MV2RBX PROC
		add r15, 2
		mov rbx, rax
		ret
	MV2RBX ENDP

	MVRAX2R14 PROC
		add r15, 2
		mov r14, rax
		ret
	MVRAX2R14 ENDP

	MVRAX2RDX PROC
		add r15, 2
		mov rdx, rax
		ret
	MVRAX2RDX ENDP

	MVRAX2R8 PROC
		add r15, 2
		mov r8, rax
		ret
	MVRAX2R8 ENDP

	MVRAX2R9 PROC
		add r15, 2
		mov r9, rax
		ret
	MVRAX2R9 ENDP

	MVRAX2RSI PROC
		add r15, 2
		mov rsi, rax
		ret
	MVRAX2RSI ENDP

	MVXMM62XMM1 PROC
		add r15, 2
		mov xmm1, xmm6
		ret
	MVXMM62XMM1 ENDP

	MVXMM62XMM2 PROC
		add r15, 2
		mov xmm2, xmm6
		ret
	MVXMM62XMM2 ENDP

	MVXMM62XMM3 PROC
		add r15, 2
		mov xmm3, xmm6
		ret
	MVXMM62XMM3 ENDP

	MVXMM62XMM4 PROC
		add r15, 2
		mov xmm4, xmm6
		ret
	MVXMM62XMM4 ENDP

	MVXMM62XMM5 PROC
		add r15, 2
		mov xmm5, xmm6
		ret
	MVXMM62XMM5 ENDP

	MVRBX2R14 PROC
		add r15, 2
		mov r14, rbx
		ret
	MVRBX2R14 ENDP

	MVRBX2RDX PROC
		add r15, 2
		mov rdx, rbx
		ret
	MVRBX2RDX ENDP

	MVRBX2R8 PROC
		add r15, 2
		mov r8, rbx
		ret
	MVRBX2R8 ENDP

	MVRBX2R9 PROC
		add r15, 2
		mov r9, rbx
		ret
	MVRBX2R9 ENDP

	MVRBX2RSI PROC
		add r15, 2
		mov rsi, rbx
		ret
	MVRBX2RSI ENDP

	MVXMM72XMM1 PROC
		add r15, 2
		mov xmm1, xmm7
		ret
	MVXMM72XMM1 ENDP

	MVXMM72XMM2 PROC
		add r15, 2
		mov xmm2, xmm7
		ret
	MVXMM72XMM2 ENDP

	MVXMM72XMM3 PROC
		add r15, 2
		mov xmm3, xmm7
		ret
	MVXMM72XMM3 ENDP

	MVXMM72XMM4 PROC
		add r15, 2
		mov xmm4, xmm7
		ret
	MVXMM72XMM4 ENDP

	MVXMM72XMM5 PROC
		add r15, 2
		mov xmm5, xmm7
		ret
	MVXMM72XMM5 ENDP

	;Load from parameters !!!!!!!!!!!!!!

	MVR142RAX PROC
		add r15, 2
		mov rax, r14
		ret
	MVR142RAX ENDP

	MVRDX2RAX PROC
		add r15, 2
		mov rax, rdx
		ret
	MVRDX2RAX ENDP

	MVR82RAX PROC
		add r15, 2
		mov rax, r8
		ret
	MVR82RAX ENDP

	MVR92RAX PROC
		add r15, 2
		mov rax, r9
		ret
	MVR92RAX ENDP

	MVRSI2RAX PROC
		add r15, 2
		mov rax, rsi
		ret
	MVRSI2RAX ENDP

	MVXMM12XMM6 PROC
		add r15, 2
		mov xmm6, xmm1
		ret
	MVXMM12XMM6 ENDP

	MVXMM22XMM6 PROC
		add r15, 2
		mov xmm6, xmm2
		ret
	MVXMM22XMM6 ENDP

	MVXMM32XMM6 PROC
		add r15, 2
		mov xmm6, xmm3
		ret
	MVXMM32XMM6 ENDP

	MVXMM42XMM6 PROC
		add r15, 2
		mov xmm6, xmm4
		ret
	MVXMM42XMM6 ENDP

	MVXMM52XMM6 PROC
		add r15, 2
		mov xmm6, xmm5
		ret
	MVXMM52XMM6 ENDP

	;; S

	MVR142RBX PROC
		add r15, 2
		mov rbx, r14
		ret
	MVR142RBX ENDP

	MVRDX2RBX PROC
		add r15, 2
		mov rbx, rdx
		ret
	MVRDX2RBX ENDP

	MVR82RBX PROC
		add r15, 2
		mov rbx, r8
		ret
	MVR82RBX ENDP

	MVR92RBX PROC
		add r15, 2
		mov rbx, r9
		ret
	MVR92RBX ENDP

	MVRSI2RBX PROC
		add r15, 2
		mov rbx, rsi
		ret
	MVRSI2RBX ENDP

	MVXMM12XMM7 PROC
		add r15, 2
		mov xmm7, xmm1
		ret
	MVXMM12XMM7 ENDP

	MVXMM22XMM7 PROC
		add r15, 2
		mov xmm7, xmm2
		ret
	MVXMM22XMM7 ENDP

	MVXMM32XMM7 PROC
		add r15, 2
		mov xmm7, xmm3
		ret
	MVXMM32XMM7 ENDP

	MVXMM42XMM7 PROC
		add r15, 2
		mov xmm7, xmm4
		ret
	MVXMM42XMM7 ENDP

	MVXMM52XMM7 PROC
		add r15, 2
		mov xmm7, xmm5
		ret
	MVXMM52XMM7 ENDP

	;;DREF

	RAX_DREF_B	PROC
		add r15, 2
		movzx rax, BYTE PTR[rax]
		ret
	RAX_DREF_B	ENDP

	RAX_DREF_W	PROC
		add r15, 2
		movzx rax, WORD PTR[rax]
		ret
	RAX_DREF_W	ENDP

	RAX_DREF_DW	PROC
		add r15, 2
		movzx rax, DWORD PTR[rax]
		ret
	RAX_DREF_DW	ENDP

	RAX_DREF_QW	PROC
		add r15, 2
		movzx rax, QWORD PTR[rax]
		ret
	RAX_DREF_QW	ENDP

	RAX_DREF_D	PROC
		add r15, 2
		movq xmm7, QWORD PTR[rax]
		ret
	RAX_DREF_D	ENDP
	
	RBX_DREF_B	PROC
		add r15, 2
		movzx rax, BYTE PTR[rbx]
		ret
	RBX_DREF_B	ENDP

	RBX_DREF_W	PROC
		add r15, 2
		movzx rax, WORD PTR[rbx]
		ret
	RBX_DREF_W	ENDP

	RBX_DREF_DW	PROC
		add r15, 2
		movzx rax, DWORD PTR[rbx]
		ret
	RBX_DREF_DW	ENDP

	RBX_DREF_QW	PROC
		add r15, 2
		movzx rax, QWORD PTR[rbx]
		ret
	RBX_DREF_QW	ENDP

	RBX_DREF_D	PROC
		add r15, 2
		movq xmm7, QWORD PTR[rbx]
		ret
	RBX_DREF_D	ENDP

	;;

	S1P PROC
		add r15, 2
		push rdx
		ret
	S1P ENDP

	S2P PROC
		add r15, 2
		sub rsp, 16
		mov QWORD PTR[rsp + 8], rdx
		mov QWORD PTR[rsp],		r8
		ret
	S2P ENDP

	S3P PROC
		add r15, 2
		sub rsp, 24
		mov QWORD PTR[rsp + 16], rdx
		mov QWORD PTR[rsp + 8 ], r8
		mov QWORD PTR[rsp]	   , r9
		ret
	S3P ENDP

	S4P PROC
		add r15, 2
		sub rsp, 32
		mov QWORD PTR[rsp + 24], rdx
		mov QWORD PTR[rsp + 16], r8
		mov QWORD PTR[rsp + 8], r9
		mov QWORD PTR[rsp]	  , rsi
		ret
	S4P ENDP

	S5P PROC
		add r15, 2
		sub rsp, 40
		mov QWORD PTR[rsp + 32], rdx
		mov QWORD PTR[rsp + 24], r8
		mov QWORD PTR[rsp + 16], r9
		mov QWORD PTR[rsp + 8] , rsi
		mov QWORD PTR[rsp]	   , rdi
		ret
	S5P ENDP

	S6P PROC
		add r15, 2
		sub rsp, 48
		mov QWORD PTR[rsp + 40], rdx
		mov QWORD PTR[rsp + 32], r8
		mov QWORD PTR[rsp + 24], r9
		mov QWORD PTR[rsp + 16], rsi
		mov QWORD PTR[rsp + 8] , rdi
		mov QWORD PTR[rsp]	   , r14
		ret
	S6P ENDP

	; Restore

	R1P PROC
		add r15, 2
		mov rdx, QWORD PTR[rsp]
		ret
	R1P ENDP
	
	R2P PROC
		add r15, 2
		mov rdx, QWORD PTR[rsp + 8]
		mov r8 , QWORD PTR[rsp]
		ret
	R2P ENDP
	
	R3P PROC
		add r15, 2
		mov rdx, QWORD PTR[rsp + 16]
		mov r8 , QWORD PTR[rsp + 8 ]
		mov r9 , QWORD PTR[rsp]
		ret
	R3P ENDP
	
	R4P PROC
		add r15, 2
		mov rdx, QWORD PTR[rsp + 24]
		mov r8 , QWORD PTR[rsp + 16]
		mov r9 , QWORD PTR[rsp + 8 ]
		mov rsi, QWORD PTR[rsp]
		ret
	R4P ENDP
	
	R5P PROC
		add r15, 2
		mov rdx, QWORD PTR[rsp + 32]
		mov r8 , QWORD PTR[rsp + 24]
		mov r9 , QWORD PTR[rsp + 16]
		mov rsi, QWORD PTR[rsp + 8 ]
		mov rdi, QWORD PTR[rsp]
		ret
	R5P ENDP
	
	R6P PROC
		add r15, 2
		mov rdx, QWORD PTR[rsp + 40]
		mov r8 , QWORD PTR[rsp + 32]
		mov r9 , QWORD PTR[rsp + 24]
		mov rsi, QWORD PTR[rsp + 16]
		mov rdi, QWPRD PTR[rsp + 8]
		mov r14, QWORD PTR[rsp]
		ret
	R6P ENDP

	;Doubles, since we have to save 16 bytes for each register, it's gonna use a lot of space

	SD1P PROC
		add r15, 2
		sub rsp, 8
		movq QWORD PTR[rsp], xmm0
		ret
	SD1P ENDP
	
	SD2P PROC
		add r15, 2
		sub rsp, 16
		movq QWORD PTR[rsp + 8],  xmm0
		movq QWORD PTR[rsp],		 xmm1
		ret
	SD2P ENDP
	
	SD3P PROC
		add r15, 2
		sub rsp, 24
		movq QWORD PTR[rsp + 16], xmm0
		movq QWORD PTR[rsp + 8 ], xmm1
		movq QWORD PTR[rsp]	   , xmm2
		ret
	SD3P ENDP
	
	SD4P PROC
		add r15, 2
		sub rsp, 32
		movq QWORD PTR[rsp + 24] xmm0
		movq QWORD PTR[rsp + 16] xmm1
		movq QWORD PTR[rsp + 8], xmm2
		movq QWORD PTR[rsp]	  , xmm3
		ret
	SD4P ENDP
	
	SD5P PROC
		add r15, 2
		sub rsp, 40
		movq QWORD PTR[rsp + 32],	xmm0
		movq QWORD PTR[rsp + 24],	xmm1
		movq QWORD PTR[rsp + 16],	xmm2
		movq QWORD PTR[rsp + 8] ,	xmm3
		movq QWORD PTR[rsp]	   ,	xmm4
		ret
	SD5P ENDP
	
	SD6P PROC
		add r15, 2
		sub rsp, 48
		movq QWORD PTR[rsp + 40], xmm0
		movq QWORD PTR[rsp + 32], xmm1
		movq QWORD PTR[rsp + 24], xmm2
		movq QWORD PTR[rsp + 16], xmm3
		movq QWORD PTR[rsp + 8] , xmm4
		movq QWORD PTR[rsp]	   , xmm5
		ret
	SD6P ENDP
	
	; Restore
	
	RD1P PROC
		add r15, 2
		movq xmm0, QWORD PTR[rsp]
		ret
	RD1P ENDP
	
	RD2P PROC
		add r15, 2
		movq xmm0, QWORD PTR[rsp + 8]
		movq xmm1, QWORD PTR[rsp]
		ret
	RD2P ENDP
	
	RD3P PROC
		add r15, 2
		movq xmm0, QWORD PTR[rsp + 16]
		movq xmm1, QWORD PTR[rsp + 8 ]
		movq xmm2, QWORD PTR[rsp]
		ret
	RD3P ENDP
	
	RD4P PROC
		add r15, 2
		movq xmm0, QWORD PTR[rsp + 24]
		movq xmm1, QWORD PTR[rsp + 16]
		movq xmm2, QWORD PTR[rsp + 8 ]
		movq xmm3, QWORD PTR[rsp]
		ret
	RD4P ENDP
	
	RD5P PROC
		add r15, 2
		movq xmm0, QWORD PTR[rsp + 32]
		movq xmm1, QWORD PTR[rsp + 24]
		movq xmm2, QWORD PTR[rsp + 16]
		movq xmm3, QWORD PTR[rsp + 8 ]
		movq xmm4, QWORD PTR[rsp]
		ret
	RD5P ENDP
	
	RD6P PROC
		add r15, 2
		movq xmm0, QWORD PTR[rsp + 40]
		movq xmm1, QWORD PTR[rsp + 32]
		movq xmm2, QWORD PTR[rsp + 24]
		movq xmm3, QWORD PTR[rsp + 16]
		movq xmm4, QWPRD PTR[rsp + 8]
		movq xmm5, QWORD PTR[rsp]
		ret
	RD6P ENDP

	RAX_B_PUSHG PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, BYTE PTR[r11 + ax]
		ret
	RAX_B_PUSHG ENDP

	RAX_W_PUSHG PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, WORD PTR[r11 + ax]
		ret
	RAX_W_PUSHG ENDP

	RAX_DW_PUSHG PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, DWORD PTR[r11 + ax]
		ret
	RAX_DW_PUSHG ENDP

	RAX_QW_PUSHG PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, QWORD PTR[r11 + ax]
		ret
	RAX_QW_PUSHG ENDP

	XMM6_D_PUSHG PROC
		add r15,2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		movq xmm6, QWORD PTR[r11 + ax]
		ret
	XMM6_D_PUSHG ENDP

	RBX_B_PUSHG PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, BYTE PTR[r11 + bx]
		ret
	RBX_B_PUSHG ENDP

	RBX_W_PUSHG PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, WORD PTR[r11 + bx]
		ret
	RBX_W_PUSHG ENDP

	RBX_DW_PUSHG PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, DWORD PTR[r11 + bx]
		ret
	RBX_DW_PUSHG ENDP

	RBX_QW_PUSHG PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, QWORD PTR[r11 + bx]
		ret
	RBX_QW_PUSHG ENDP

	XMM7_D_PUSHG PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		movq xmm7, QWORD PTR[r11 + bx]
		ret
	XMM7_D_PUSHG ENDP

	;;GN

	RAX_B_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, BYTE PTR[rax + 20h + r11w]
		ret
	RAX_B_PUSHGN ENDP

	RAX_W_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, WORD PTR[rax + 20h + r11w]
		ret
	RAX_W_PUSHGN ENDP

	RAX_DW_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, DWORD PTR[rax + 20h + r11w]
		ret
	RAX_DW_PUSHGN ENDP

	RAX_QW_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 20h + r11w]
		ret
	RAX_QW_PUSHGN ENDP

	XMM6_D_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		movq xmm6, QWORD PTR[rax + 20h + r11w]
		ret
	XMM6_D_PUSHGN ENDP

	RBX_B_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, BYTE PTR[rbx + 20h + r11w]
		ret
	RBX_B_PUSHGN ENDP

	RBX_W_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, WORD PTR[rbx + 20h + r11w]
		ret
	RBX_W_PUSHGN ENDP

	RBX_DW_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, DWORD PTR[rbx + 20h + r11w]
		ret
	RBX_DW_PUSHGN ENDP

	RBX_QW_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 20h + r11w]
		ret
	RBX_QW_PUSHGN ENDP

	XMM7_D_PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		movq xmm7, QWORD PTR[rbx + 20h + r11w]
		ret
	XMM7_D_PUSHGN ENDP

	;;GNS

	RAX_B_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, BYTE PTR[rax + r11w]
		ret
	RAX_B_PUSHGNS ENDP

	RAX_W_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, WORD PTR[rax + r11w]
		ret
	RAX_W_PUSHGNS ENDP

	RAX_DW_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, DWORD PTR[rax + r11w]
		ret
	RAX_DW_PUSHGNS ENDP

	RAX_QW_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, QWORD PTR[rax + r11w]
		ret
	RAX_QW_PUSHGNS ENDP

	XMM6_D_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		movq xmm6, QWORD PTR[rax + r11w]
		ret
	XMM6_D_PUSHGNS ENDP

	RBX_B_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, BYTE PTR[rbx + r11w]
		ret
	RBX_B_PUSHGNS ENDP

	RBX_W_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, WORD PTR[rbx + r11w]
		ret
	RBX_W_PUSHGNS ENDP

	RBX_DW_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, DWORD PTR[rbx + r11w]
		ret
	RBX_DW_PUSHGNS ENDP

	RBX_QW_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, QWORD PTR[rbx + r11w]
		ret
	RBX_QW_PUSHGNS ENDP

	XMM7_D_PUSHGNS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		movq xmm7, QWORD PTR[rbx + r11w]
		ret
	XMM7_D_PUSHGNS ENDP

	;;GS

	RAX_B_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, BYTE PTR[rax + r11w]
		ret
	RAX_B_PUSHGS ENDP

	RAX_W_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, WORD PTR[rax + r11w]
		ret
	RAX_W_PUSHGS ENDP

	RAX_DW_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, DWORD PTR[rax + r11w]
		ret
	RAX_DW_PUSHGS ENDP

	RAX_QW_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, QWORD PTR[rax + r11w]
		ret
	RAX_QW_PUSHGS ENDP

	XMM6_D_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		movq xmm6, QWORD PTR[rax + r11w]
		ret
	XMM6_D_PUSHGS ENDP

	RBX_B_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, BYTE PTR[rbx + r11w]
		ret
	RBX_B_PUSHGS ENDP

	RBX_W_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, WORD PTR[rbx + r11w]
		ret
	RBX_W_PUSHGS ENDP

	RBX_DW_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, DWORD PTR[rbx + r11w]
		ret
	RBX_DW_PUSHGS ENDP

	RBX_QW_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, QWORD PTR[rbx + r11w]
		ret
	RBX_QW_PUSHGS ENDP

	XMM7_D_PUSHGS PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		movq xmm7, QWORD PTR[rbx + r11w]
		ret
	XMM7_D_PUSHGS ENDP

	;;THIS VERSION

	RAX_B_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, BYTE PTR[r11 + ax]
		ret
	RAX_B_PUSHG_T ENDP

	RAX_W_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, WORD PTR[r11 + ax]
		ret
	RAX_W_PUSHG_T ENDP

	RAX_DW_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, DWORD PTR[r11 + ax]
		ret
	RAX_DW_PUSHG_T ENDP

	RAX_QW_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		mov rax, QWORD PTR[r11 + ax]
		ret
	RAX_QW_PUSHG_T ENDP

	XMM6_D_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rax + 8h]	;Globals
		mov ax, WORD PTR[r15]
		add r15, 2
		movq xmm6, QWORD PTR[r11 + ax]
		ret
	XMM6_D_PUSHG_T ENDP

	RBX_B_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, BYTE PTR[r11 + bx]
		ret
	RBX_B_PUSHG_T ENDP

	RBX_W_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, WORD PTR[r11 + bx]
		ret
	RBX_W_PUSHG ENDP

	RBX_DW_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, DWORD PTR[r11 + bx]
		ret
	RBX_DW_PUSHG_T ENDP

	RBX_QW_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		mov rbx, QWORD PTR[r11 + bx]
		ret
	RBX_QW_PUSHG_T ENDP

	XMM7_D_PUSHG_T PROC
		add r15,2
		mov r11, QWORD PTR [rbx + 8h]	;Globals
		mov bx, WORD PTR[r15]
		add r15, 2
		movq xmm7, QWORD PTR[r11 + bx]
		ret
	XMM7_D_PUSHG_T ENDP

	;;GN

	RAX_B_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, BYTE PTR[rax + 20h + r11w]
		ret
	RAX_B_PUSHGN_T ENDP

	RAX_W_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, WORD PTR[rax + 20h + r11w]
		ret
	RAX_W_PUSHGN_T ENDP

	RAX_DW_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, DWORD PTR[rax + 20h + r11w]
		ret
	RAX_DW_PUSHGN_T ENDP

	RAX_QW_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 20h + r11w]
		ret
	RAX_QW_PUSHGN_T ENDP

	XMM6_D_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		movq xmm6, QWORD PTR[rax + 20h + r11w]
		ret
	XMM6_D_PUSHGN_T ENDP

	RBX_B_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, BYTE PTR[rbx + 20h + r11w]
		ret
	RBX_B_PUSHGN_T ENDP

	RBX_W_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, WORD PTR[rbx + 20h + r11w]
		ret
	RBX_W_PUSHGN_T ENDP

	RBX_DW_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, DWORD PTR[rbx + 20h + r11w]
		ret
	RBX_DW_PUSHGN_T ENDP

	RBX_QW_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 20h + r11w]
		ret
	RBX_QW_PUSHGN_T ENDP

	XMM7_D_PUSHGN_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		movq xmm7, QWORD PTR[rbx + 20h + r11w]
		ret
	XMM7_D_PUSHGN_T ENDP

	;;GNS

	RAX_B_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, BYTE PTR[rax + r11w]
		ret
	RAX_B_PUSHGNS_T ENDP

	RAX_W_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, WORD PTR[rax + r11w]
		ret
	RAX_W_PUSHGNS_T ENDP

	RAX_DW_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, DWORD PTR[rax + r11w]
		ret
	RAX_DW_PUSHGNS_T ENDP

	RAX_QW_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		mov rax, QWORD PTR[rax + r11w]
		ret
	RAX_QW_PUSHGNS_T ENDP

	XMM6_D_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 60h]
		movq xmm6, QWORD PTR[rax + r11w]
		ret
	XMM6_D_PUSHGNS_T ENDP

	RBX_B_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, BYTE PTR[rbx + r11w]
		ret
	RBX_B_PUSHGNS_T ENDP

	RBX_W_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, WORD PTR[rbx + r11w]
		ret
	RBX_W_PUSHGNS_T ENDP

	RBX_DW_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, DWORD PTR[rbx + r11w]
		ret
	RBX_DW_PUSHGNS_T ENDP

	RBX_QW_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		mov rbx, QWORD PTR[rbx + r11w]
		ret
	RBX_QW_PUSHGNS_T ENDP

	XMM7_D_PUSHGNS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 60h]
		movq xmm7, QWORD PTR[rbx + r11w]
		ret
	XMM7_D_PUSHGNS_T ENDP

	;;GS

	RAX_B_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, BYTE PTR[rax + r11w]
		ret
	RAX_B_PUSHGS_T ENDP

	RAX_W_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, WORD PTR[rax + r11w]
		ret
	RAX_W_PUSHGS_T ENDP

	RAX_DW_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, DWORD PTR[rax + r11w]
		ret
	RAX_DW_PUSHGS_T ENDP

	RAX_QW_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		mov rax, QWORD PTR[rax + r11w]
		ret
	RAX_QW_PUSHGS_T ENDP

	XMM6_D_PUSHGS_T PROC
		add r15,2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rax, QWORD PTR[rax + 58h]
		movq xmm6, QWORD PTR[rax + r11w]
		ret
	XMM6_D_PUSHGS_T ENDP

	RBX_B_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, BYTE PTR[rbx + r11w]
		ret
	RBX_B_PUSHGS_T ENDP

	RBX_W_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, WORD PTR[rbx + r11w]
		ret
	RBX_W_PUSHGS_T ENDP

	RBX_DW_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, DWORD PTR[rbx + r11w]
		ret
	RBX_DW_PUSHGS_T ENDP

	RBX_QW_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		mov rbx, QWORD PTR[rbx + r11w]
		ret
	RBX_QW_PUSHGS_T ENDP

	XMM7_D_PUSHGS_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2					;increment IP
		mov rbx, QWORD PTR[rbx + 58h]
		movq xmm7, QWORD PTR[rbx + r11w]
		ret
	XMM7_D_PUSHGS_T ENDP


	PUSHGS_T PROC
		add r15, 2
		mov r11, QWORD PTR [r10 + 60h]	;native pointer in class
		mov ax, WORD PTR[r15]		;Get offset
		add r15, 2						;increment IP
		mov rax, QWORD PTR[r11 + ax*8h]
		and rax, QWORD PTR[r15]
		add r15, 8
		ret
	PUSHGS_T ENDP

	PUSHGN PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2						;increment IP
		mov rax, QWORD PTR[r11w + 20h + rax*8h]
		and rax, QWORD PTR[r15]
		add r15, 8
		ret
	PUSHGN ENDP

	PUSHGS PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 60h]	;Static offset
		mov ax, WORD PTR[r15]		;Data offset
		add r15, 2						;increment IP
		mov rax, QWORD PTR[r11 + ax*8h]
		and rax, QWORD PTR[r15]
		add r15, 8
		ret
	PUSHGS ENDP

	LOADCLASS PROC
		push rcx
		push rdx	;Shouldn't need to save more at this point
		mov rcx, QWORD PTR[r15 + 1]	;Read package ID
		mov rdx, QWORD PTR[r15 + 9] ;Read Class ID
		call ?findClass@HClass@@SAPEAV1@_K@Z
		mov QWORD PTR[r15 + 1], rax	;Insert class address into the bytecode
		mov BYTE PTR[r15], 24h	;Replace with PUSHC instruction
		pop rdx
		pop rcx
		ret
	LOADCLASS ENDP

	RAX_PUSHC PROC
		add r15, 2
		mov rax, QWORD PTR[r15]
		add r15, 10h	;Compensate for the extra 8 bytes
		ret
	RAX_PUSHC ENDP

	RBX_PUSHC PROC
		add r15, 2
		mov rbx, QWORD PTR[r15]
		add r15, 10h	;Compensate for the extra 8 bytes
		ret
	RBX_PUSHC ENDP

	RAX_B_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rax, BYTE PTR[rbp - r11w] ;base pointer is right after the locals, so offset must be inverse
		add r15, 2 ;Offset
		ret
	RAX_B_PUSHL ENDP

	RBX_B_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rbx, BYTE PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RBX_B_PUSHL ENDP

	RAX_W_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rax, WORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RAX_W_PUSHL ENDP

	RBX_W_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rbx, WORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RBX_W_PUSHL ENDP

	RAX_DW_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rax, DWORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RAX_DW_PUSHL ENDP

	RBX_DW_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rbx, DWORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RBX_DW_PUSHL ENDP

	RAX_QW_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rax, QWORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RAX_QW_PUSHL ENDP

	RBX_QW_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		mov rbx, QWORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RBX_QW_PUSHL ENDP

	RAX_D_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		movq xmm6, QWORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RAX_D_PUSHL ENDP

	RBX_D_PUSHL PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		movq xmm7, QWORD PTR[rbp - r11w]
		add r15, 2 ;Offset
		ret
	RBX_D_PUSHL ENDP

	; LI

	RBX_PUSHLI PROC
		add r15, 2
		mov rbx, QWORD PTR [r15]
		add r15, 8
		ret
	RBX_PUSHLI ENDP

	XMM7_PUSHLI PROC
		add r15, 2
		movq xmm7, QWORD PTR [r15]
		add r15, 8
		ret
	XMM7_PUSHLI ENDP

	; LP

	RAX_PUSHLP PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		lea rax, QWORD PTR[rbp - r11w] ;base pointer is right after the locals, so offset must be inverse
		ret
	RAX_PUSHLP ENDP

	RBX_PUSHLP PROC
		add r15, 2
		mov r11w, WORD PTR[r15]	;offset
		lea rbx, QWORD PTR[rbp - r11w] ;base pointer is right after the locals, so offset must be inverse
		ret
	RBX_PUSHLP ENDP

	; GP

	RAX_PUSHGP PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 8h]	;Global pointer
		mov ax, WORD PTR[r15]
		add r15, 2
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RBX_PUSHGP ENDP

	RAX_PUSHGP PROC
		add r15, 2
		mov r11, QWORD PTR [rbx + 8h]	;Global pointer
		mov bx, WORD PTR[r15]
		add r15, 2
		lea rbx, QWORD PTR[r11 + bx*8h]
		ret
	RBX_PUSHGP ENDP

	; GNP

	RAX_PUSHGNP PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11w + 20h + rax*8h]
		ret
	RAX_PUSHGNP ENDP

	RBX_PUSHGNP PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2						;increment IP
		lea rbx, QWORD PTR[r11w + 20h + rbx*8h]
		ret
	RBX_PUSHGNP ENDP

	; GSP

	RAX_PUSHGSP PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 58h]	;Static offset
		mov ax, WORD PTR[r15]			;Data offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RAX_PUSHGSP ENDP

	RBX_PUSHGSP PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 58h]	;Static offset
		mov ax, WORD PTR[r15]			;Data offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RBX_PUSHGSP ENDP

	; GNSP

	RAX_PUSHGNSP PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 60h]	;Static offset
		mov ax, WORD PTR[r15]			;Data offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RAX_PUSHGNSP ENDP

	RBX_PUSHGNSP PROC
		add r15, 2
		mov r11, QWORD PTR [rax + 60h]	;Static offset
		mov ax, WORD PTR[r15]			;Data offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RBX_PUSHGNSP ENDP

	;This context

	RAX_PUSHGP_T PROC
		add r15, 2
		mov r11, QWORD PTR [r13 + 8h]	;Global pointer
		mov ax, WORD PTR[r15]
		add r15, 2
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RAX_PUSHGP_T ENDP

	RBX_PUSHGP_T PROC
		add r15, 2
		mov r11, QWORD PTR [r13 + 8h]	;Global pointer
		mov bx, WORD PTR[r15]
		add r15, 2
		lea rbx, QWORD PTR[r11 + bx*8h]
		ret
	RBX_PUSHGP_T ENDP

	;	GNP

	RAX_PUSHGNP_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11w + 20h + r13*8h]
		ret
	RAX_PUSHGNP_T ENDP

	RBX_PUSHGNP_T PROC
		add r15, 2
		mov r11w, WORD PTR[r15]		;Get offset
		add r15, 2						;increment IP
		lea rbx, QWORD PTR[r11w + 20h + r13*8h]
		ret
	RBX_PUSHGNP_T ENDP

	;	GSP

	RAX_PUSHGSP_T PROC
		add r15, 2
		mov r11, QWORD PTR [r10 + 58h]	;Static offset
		mov ax, WORD PTR[r15]		;Data offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RAX_PUSHGSP_T ENDP

	RBX_PUSHGSP_T PROC
		add r15, 2
		mov r11, QWORD PTR [r10 + 58h]	;Static offset
		mov bx, WORD PTR[r15]		;Data offset
		add r15, 2						;increment IP
		lea rbx, QWORD PTR[r11 + bx*8h]
		ret
	RBX_PUSHGSP_T ENDP

	;	GNSP

	RAX_PUSHGNSP_T PROC
		add r15, 2
		mov r11, QWORD PTR [r10 + 60h]	;Static offset
		mov ax, WORD PTR[r15]		;Data offset
		add r15, 2						;increment IP
		lea rax, QWORD PTR[r11 + ax*8h]
		ret
	RAX_PUSHGNSP_T ENDP

	RBX_PUSHGNSP_T PROC
		add r15, 2
		mov r11, QWORD PTR [r10 + 60h]	;Static offset
		mov bx, WORD PTR[r15]		;Data offset
		add r15, 2						;increment IP
		lea rbx, QWORD PTR[r11 + bx*8h]
		ret
	RBX_PUSHGNSP_T ENDP

	CALLNAT PROC
	add r15, 2
	push rcx
	mov rcx, rax	;this pointer.. kinda messy
	mov ax, WORD PTR   [r15]
	mov rbx, QWORD PTR [rcx + 10h]
	call [rbx + ax]	;return value goes into rax just as we want it 
	pop rcx
	ret
	CALLNAT ENDP

	CALLNAT_T PROC
		add r15, 2
		push rcx		;this is a reference to the thread during script execution
		mov rcx, r13	;this pointer.. kinda messy
		mov ax, WORD PTR   [r15]
		mov rbx, QWORD PTR [rcx + 10h]
		call [rbx + ax]	;return value goes into rax just as we want it 
		pop rcx
		ret
	CALLNAT_T ENDP

	CALLVIRT PROC
		push r15
		push r13
		push r10
		push rbp
		mov rbp, rsp
		mov r13, rax	;Set this pointer
		mov	rax, QWORD PTR [r13 + 24]	 ;vftable pointer
		mov r15, QWORD PTR [rax + r8*8h] ;function pointer
		mov r10, QWORD PTR [r13 + 8h]	 ;the class
		sub sp, WORD PTR [r15+18h]		 ;allocate locals
		mov r15, QWORD PTR [r15 + 8h]	 ;redirect to instruction pointers
		test BYTE PTR [r15], 255		 ; < 256?
		jz SHORT @ends
		@executes:
		mov r11w, WORD PTR [r15]
		call [r12 + r11w]	;Call by offset and not index, skips an extra multi.
		test BYTE PTR[r15], 255		
		jnz SHORT @executes
		@ends:
		mov rsp, rbp
		pop rbp
		pop r10
		pop r13
		pop r15
		ret
	CALLVIRT ENDP

	CALLVIRT_T PROC
		push r15
		push rbp
		mov rbp, rsp
		mov r13, rax	;Set this pointer
		mov	rax, QWORD PTR [r13 + 24]	 ;vftable pointer
		mov r15, QWORD PTR [rax + r8*8h] ;function pointer
		mov r10, QWORD PTR [r13 + 8h]	 ;the class
		sub sp, WORD PTR [r15+18h]		 ;allocate locals
		mov r15, QWORD PTR [r15 + 8h]	 ;redirect to instruction pointers
		test BYTE PTR [r15], 255		 ; < 256?
		jz SHORT @ends
		@executes:
		mov r11w, WORD PTR [r15]
		call [r12 + r11w]	;Call by offset and not index, skips an extra multi.
		test BYTE PTR[r15], 255		
		jnz SHORT @executes
		@ends:
		mov rsp, rbp
		pop rbp
		pop r15
		ret
	CALLVIRT_T ENDP

	CALLSTATIC PROC
		push r15
		push r10
		push rbp
		mov rbp, rsp
		mov r10, rax					 ;Set class pointer
		mov	rax, QWORD PTR [r10 + 30h]	 ;vftable pointer
		mov r15, QWORD PTR [rax + r8*8h] ;function pointer
		sub sp, WORD PTR [r15+18h]		 ;allocate locals
		mov r15, QWORD PTR [r15 + 8h]	 ;redirect to instruction pointers
		test BYTE PTR [r15], 255		 ; < 256?
		jz SHORT @ends
		@executes:
		mov r11w, WORD PTR [r15]
		call QWORD PTR[r12 + r11w]		;Call by offset and not index, skips an extra multi.
		test BYTE PTR[r15], 255		
		jnz SHORT @executes
		@ends:
		mov rsp, rbp
		pop rbp
		pop r10
		pop r15
		ret
	CALLSTATIC ENDP

	CALLSTATIC_T PROC
		push r15
		push rbp
		mov rbp, rsp
		mov	rax, QWORD PTR [r10 + 30h]	 ;vftable pointer
		mov r15, QWORD PTR [rax + r8*8h] ;function pointer
		sub sp, WORD PTR [r15+18h]		 ;allocate locals
		mov r15, QWORD PTR [r15 + 8h]	 ;redirect to instruction pointers
		test BYTE PTR [r15], 255		 ; < 256?
		jz SHORT @ends
		@executes:
		mov r11w, WORD PTR [r15]
		call QWORD PTR[r12 + r11w]		;Call by offset and not index, skips an extra multi.
		test BYTE PTR[r15], 255		
		jnz SHORT @executes
		@ends:
		mov rsp, rbp
		pop rbp
		pop r15
		ret
	CALLSTATIC_T ENDP

	CALLSTATNAT_T PROC
		add r15, 2
		push rcx		;this is a reference to the thread during script execution
		mov rcx, r10	;class pointer
		mov ax, WORD PTR   [r15]
		mov rbx, QWORD PTR [rcx + 50h]
		call [rbx + ax]	;return value goes into rax just as we want it 
		pop rcx
		ret
	CALLSTATNAT_T ENDP

	 CALLSTATNAT PROC
		add r15, 2
		push rcx
		mov rcx, rax	;class pointer.. kinda messy
		mov ax, WORD PTR   [r15]
		mov rbx, QWORD PTR [rcx + 50h]
		call [rbx + ax]	;return value goes into rax just as we want it 
		pop rcx
		ret
	CALLSTATNAT ENDP

	;;OPERATORS
	ASSI PROC
		add r15, 2
		mov QWORD PTR [rax], rbx
		mov rax, rbx
		ret
	ASSI ENDP

	ASSIB PROC
		add r15, 2
		mov BYTE PTR [rax], bl
		mov al, bl
		ret
	ASSIB ENDP

	ASSIW PROC
		add r15, 2
		mov WORD PTR [rax], bx
		mov ax, bx
		ret
	ASSIW ENDP

	ASSIDW PROC
		add r15, 2
		mov DWORD PTR [rax], ebx
		mov eax, ebx
		ret
	ASSIDW ENDP

	ASSID PROC
		add r15, 2
		movq QWORD PTR [rax], xmm7
		ret
	ASSID ENDP

	ASSIS PROC
		add r15, 2
		push rcx ;save thread
		push rdx
		mov rcx, rax
		mov rdx, rbx
		call ??4HString@@QEAAAEAV0@AEBV0@@Z
		pop rdx
		pop rdx
		ret
	ASSIS ENDP

	ASSISI PROC
		add r15, 2
		push rcx ;save thread
		push rdx
		mov rcx, rax
		mov rdx, rbx
		call ??4HString@@QEAAAEAV0@_J@Z
		pop rdx
		pop rdx
		ret
	ASSISI ENDP

	ASSISD PROC
		add r15, 2
		push rcx
		sub rsp, 16
		mov XMMWORD PTR [rsp], rcx
		movq xmm0, xmm6
		call ??4HString@@QEAAAEAV0@N@Z
		add rsp, 16
		pop rcx
		ret
	ASSISD ENDP

	ASSISO PROC
		add r15, 2
		;rax = dest string, rbx = object
		mov rbx, QWORD PTR[rbx + 28h]
		call ASSIS
		ret
	ASSISO ENDP

	ASSIO PROC
		add r15, 2
					;rax pointer to pointer, rbx pointer to object
		mov QWORD PTR[rax], rbx
		ret
	ASSIO ENDP

	ITOD PROC
		add r15, 2
		cvtsi2sd xmm6, rbx
		ret
	ITOD ENDP

	DTOI PROC
		add r15, 2
		cvttsd2si rax, xmm7
		ret
	DTOI ENDP

	ITOS PROC
		add r15, 2
	;get string from thread stack
		mov rbx, QWORD PTR [rcx + 16]	;Strings
		add rbx, WORD PTR[rcx + 8]		;Aligned to 16 * index
		add WORD PTR[rcx + 8], 16
		and WORD PTR[rcx + 8], 4095
		push rcx
		push rdx
		mov rcx,rbx
		mov rdx, rax
		call ??4HString@@QEAAAEAV0@_J@Z
		pop rdx
		pop rcx
		ret
	ITOS ENDP

	UITOS PROC
		mov rbx, QWORD PTR [rcx + 16]	;Strings
		add rbx, WORD PTR[rcx + 8]		;Aligned to 16 * index
		add WORD PTR[rcx + 8], 16
		and WORD PTR[rcx + 8], 4095
		push rcx
		push rdx
		mov rcx,rbx
		mov rdx, rax
		call ??4HString@@QEAAAEAV0@_K@Z
		pop rdx
		pop rcx
		ret
	UITOS ENDP

	DTOS PROC
		mov rbx, QWORD PTR [rcx + 16]	;Strings
		add rbx, WORD PTR[rcx + 8]		;Aligned to 16 * index
		add WORD PTR[rcx + 8], 16
		and WORD PTR[rcx + 8], 4095
		push rcx
		sub rsp, 8
		movq QWORD PTR[rsp], xmm0
		mov rcx,rbx
		mov rdx, rax
		call ??4HString@@QEAAAEAV0@_K@Z
		movzx xmm0, QWORD PTR[rsp]
		add rsp,8
		pop rcx
		ret
	DTOS ENDP

	;Init local str to null, very efficient
	LSTRNULL PROC
		add r15, 2
		mov ax, WORD PTR[r15]
		mov XMMWORD PTR[rbp + ax], 0	;Assign the wstring and count to 0 
		add r15, 2
		ret
	LSTRNULL ENDP
	
	ASSIZ	PROC
		add r15, 2
		mov QWORD PTR[rax], 0
		ret
	ASSIZ	ENDP

	ASSIBZ	PROC
		add r15, 2
		mov BYTE PTR[rax], 0
		ret
	ASSIBZ	ENDP

	ASSIWZ PROC
		add r15, 2
		mov WORD PTR[rax], 0
		ret
	ASSIWZ ENDP

	ASSIDWZ PROC
		add r15, 2
		mov DWORD PTR[rax], 0
		ret
	ASSIDWZ ENDP

	ASSIDZ PROC
		add r15, 2
		mov QWORD PTR[rax], 0
		ret
	ASSIDZ ENDP

	;Stack based string

	DUPSTR PROC
		add r15, 2
		sub rsp, 16	;Allocate the string obj
		push rcx
		push rbx
		mov r11, QWORD PTR[rax]	;save source string ptr
		movzx rcx, WORD PTR[rax + 8] ;Copy length
		mov WORD PTR[rsp + 8], rcx
		call malloc				;Allocate space for string
		mov QWORD PTR[rsp],rax	;Assign string pointer
		mov rax, QWORD PTR[rsp]	;destination string
		test rcx, rcx	;rcx num chars
		jz @END
		@DO:
		mov bx, WORD PTR[rax]
		mov WORD PTR[r11], bx
		add rax, 2
		add r11, 2
		sub rcx, 1
		jnz @DO
		@END:
		mov WORD PTR[rax], 0	;Null terminating
		pop rbx
		pop rcx
		ret
	DUPSTR ENDP

	CLEARSTR PROC
		add r15, 2
		push rcx
		mov rcx, QWORD PTR[rsp]
		call free
		pop rcx
		add rsp, 16
		ret
	CLEARSTR ENDP

	;OPERATORS

	ADD	PROC
		add r15, 2
		add rax, rbx
		ret
	ADD	ENDP

	ADDD PROC
		add r15, 2
		addsd xmm6, xmm7
		ret
	ADDD ENDP

	ADDID PROC
		add r15, 2
		cvtsq2sd rbx, xmm7
		addsd xmm6, xmm7
		ret
	ADDID ENDP

	ADDDI PROC
		add r15, 2
		cvtsd2sq xmm7, rbx
		add rax, rbx
		ret
	ADDDI ENDP

	RegisterFunction PROC
		mov QWORD PTR[rcx], rdx
		ret
	RegisterFunction ENDP


_TEXT ENDS

_DATA SEGMENT

_DATA ENDS

END