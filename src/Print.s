; Print.s
; Student names: Xinyuan Pan & Chimezie Iwuanyanwu
; Last modification date: 3/29
; Runs on TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
string EQU 0 ;maximum value has 10 digits, so 10 bytes 
; from lower address to higher address, lower digit to higher digit
	PUSH {R4, R5, R6, LR}
	SUB  SP, #16 ; need to be multiple of 4, so the extra 2 bytes are useless
	MOV  R1, R0  ; R1 stores the number to be converted
	MOV  R3, #0  ; R3 offsets the string
	MOV  R6, #10 ; used for division and mul
	; get the lowest digit and convert to ASCII
numtostr
	UDIV R4, R1, R6
	MUL  R5, R4, R6
	SUB  R2, R1, R5  ; the remainder in R2, which is the lowest digit
	MOV  R1, R4 ; put the number left into R1
	ADD  R2, #0x30 ; convert R2 to ASCII value
	; now put R2 to the corresponding place in stack
	STRB  R2, [SP,R3] 
	; increment R3
	ADD  R3,#1
	; if R1 = 0 then we've done converting the number to string
	CMP  R1, #0
	BNE  numtostr
	
	; start printing now
	; print from higher address to lower address

print	
	SUB   R3, #1 ; must minus first!!! 
	LDRB   R0, [SP, R3] ;only get 1 byte (the other locations are empty anyways, since it's a char)
	PUSH {R3,R4} ; have to do this, cuz inside that function it doesnt save R3
	BL    ST7735_OutChar
	POP  {R3,R4}
	CMP   R3, #0
	BNE   print
	; if R3 is 0, then done printing
	ADD   SP, #16
	POP  {R4, R5, R6, LR}
    BX   LR
	
;* * * * * * * * End of LCD_OutDec * * * * * * * *

	

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
char1	EQU	0
dec		EQU	1
char2	EQU	2
char3	EQU 3
char4	EQU 4
dot		EQU	0x2E			
ast		EQU	0x2A
	 PUSH {R4, R5, R6, LR}
	 SUB  SP, #8
	 ; the case that number is >9999
	 MOV  R3, #9999
	 CMP  R0, R3
	 BHI  aboveRange
	 
	 MOV  R6, #10 ; used for division
	 
     UDIV R1, R0, R6
	 MUL  R4, R1, R6
	 SUB  R2, R0, R4 ; R2 is the remainder
	 MOV  R0, R1 ; update R0
	 ADD  R2, #0x30
	 STRB R2, [SP, #char4]

	 UDIV R1, R0, R6
	 MUL  R4, R1, R6
	 SUB  R2, R0, R4 ; R2 is the remainder
	 MOV  R0, R1 ; update R0
	 ADD  R2, #0x30
	 STRB R2, [SP, #char3]
	 
	 UDIV R1, R0, R6
	 MUL  R4, R1, R6
	 SUB  R2, R0, R4 ; R2 is the remainder
	 MOV  R0, R1 ; update R0
	 ADD  R2, #0x30
	 STRB R2, [SP, #char2]
	 
	 MOV  R2, #dot
	 STRB R2, [SP, #dec]
	 
	 UDIV R1, R0, R6
	 MUL  R4, R1, R6
	 SUB  R2, R0, R4 ; R2 is the remainder
	 MOV  R0, R1 ; update R1
	 ADD  R2, #0x30
	 STRB R2, [SP, #char1]
	 B    print2
	 
aboveRange
	 MOV   R2, #ast
	 STRB  R2, [SP,#char1]
	 STRB  R2, [SP,#char2]
	 STRB  R2, [SP,#char3]
	 STRB  R2, [SP,#char4]
	 MOV   R2, #dot
	 STRB  R2, [SP,#dec]

print2
	 LDRB  R0, [SP,#char1]
	 BL		ST7735_OutChar
	 LDRB  R0, [SP,#dec]
	 BL		ST7735_OutChar
	 LDRB  R0, [SP,#char2]
	 BL		ST7735_OutChar
	 LDRB  R0, [SP,#char3]
	 BL		ST7735_OutChar
	 LDRB  R0, [SP,#char4]
	 BL		ST7735_OutChar
	 
	 ADD  SP, #8
	 POP {R4, R5, R6, LR}
     BX   LR

;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
