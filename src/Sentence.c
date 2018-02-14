
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Sentence.h"
#include "../inc/ST7735.h"
#include "../inc/Sentence_Images.h"



#define ASCII 0x41 //start of capital letters

void drawSentence (char ASCIIcharacter[20], uint8_t length, int16_t drawX, int16_t drawY){
	uint8_t Index;
	for(Index = 0; Index < length; Index++){
		if(ASCIIcharacter[Index] != 0x20)
		drawCharacter(ASCIIcharacter[Index] - ASCII, drawX, drawY);
		drawY -= 6;
	}
}

void drawNumber (uint16_t number, uint8_t digits, int16_t drawX, int16_t drawY){ 
	uint8_t drawDigits[10];
	uint8_t digitPt;
	for(digitPt = 0; digitPt < digits - 1; digitPt++){
		uint16_t divisorPt, divisor;
		divisor = 1;
		for(divisorPt = 0; divisorPt < digits - digitPt - 1; divisorPt++)
			divisor *= 10;
		drawDigits[digitPt] = number / divisor;
		drawCharacter (drawDigits[digitPt] + 26, drawX, drawY);
		number %= divisor;
		drawY -= 6; 
	}
	drawDigits[digitPt] = number;
	drawCharacter (drawDigits[digitPt] + 26, drawX, drawY);
}

void drawCharacter (uint8_t Index, int16_t drawX, int16_t drawY){
	if(Index < 26){
		ST7735_DrawBitmap(drawX, drawY, getLetter(Index), 5, 5);
	}
	else{
		ST7735_DrawBitmap(drawX, drawY, getNumber(Index - 26), 5, 5);
	}
}

int16_t centerY(uint8_t length){
	return 80 + ((length * 6)/2);
}
int16_t floatRight(uint8_t length){
	return (length * 6) + 1;
}
