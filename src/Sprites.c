
#include <stdint.h>
#include "Sprites.h"
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Map.h"
#include "Grid.h"
#include "Timer0.h"
#include "Sprite_Images.h"

struct Sprite_t {
	const unsigned short * currentCharacter;
	
	const unsigned short * CharacterN;
	const unsigned short * CharacterNA;
	const unsigned short * CharacterE;
	const unsigned short * CharacterEA;
	const unsigned short * CharacterS;
	const unsigned short * CharacterSA;
	const unsigned short * CharacterW;
	const unsigned short * CharacterWA;
	
	const unsigned short * AttackN;
	const unsigned short * AttackNAni1;
	const unsigned short * AttackNAni2;
	const unsigned short * AttackE;
	const unsigned short * AttackEAni1;
	const unsigned short * AttackEAni2;
	const unsigned short * AttackS;
	const unsigned short * AttackSAni1;
	const unsigned short * AttackSAni2;
	const unsigned short * AttackW;
	const unsigned short * AttackWAni1;
	const unsigned short * AttackWAni2;
	
	uint8_t width;
	uint8_t height;
	uint8_t attackWidth;
	uint8_t attackHeight;
};

uint8_t updateSprite = 0;
uint8_t spriteAttacking = 0;
void spriteInit(uint8_t Index, uint8_t xPosition, uint8_t yPosition, uint8_t direction, uint8_t label){
	setGridPosition(label, xPosition, yPosition);
	if(Index == 4){
		setGridPosition(label, xPosition, yPosition - 1);
		setGridPosition(label, xPosition + 1, yPosition);
		setGridPosition(label, xPosition + 1, yPosition - 1);
		setGridPosition(label, xPosition + 1, yPosition - 2);
		setGridPosition(label, xPosition + 2, yPosition);
		setGridPosition(label, xPosition + 2, yPosition - 1);
		setGridPosition(label, xPosition + 2, yPosition - 2);
	}
	drawSprite(Index, label, gridPositionX(xPosition), gridPositionY(yPosition), direction, 0);
}
void drawSprite (uint8_t Index, uint8_t label, int16_t drawX, int16_t drawY, uint8_t direction, uint8_t spriteAnimationCounter){
	struct Sprite_t current = getSprite(Index);
	setSpriteAnimation(Index, direction, spriteAnimationCounter);
	if(spriteAttacking){
		if(direction == 0)
			SpriteDraw(drawX - (current.attackWidth - current.width), drawY, current.currentCharacter, current.attackWidth, current.attackHeight);
		else if(direction == 1)
			SpriteDraw(drawX, drawY, current.currentCharacter, current.attackHeight, current.attackWidth);
		else if(direction == 2)
			SpriteDraw(drawX, drawY, current.currentCharacter, current.attackWidth, current.attackHeight);
		else
			SpriteDraw(drawX, drawY + (current.attackWidth - current.height), current.currentCharacter, current.attackHeight, current.attackWidth);
		}
	else
		SpriteDraw(drawX, drawY, current.currentCharacter, current.width, current.height);
}
void setSpriteLocation (uint8_t Index, uint8_t * prevXPosition, uint8_t * prevYPosition, 
												int16_t * drawX, int16_t * drawY, uint8_t direction, uint8_t label, 
												uint8_t * spriteAnimationCounter, uint8_t * isMoving, uint8_t spritesMoving){
	setTimer(spritesMoving);
	int16_t newXpos;
	int16_t newYpos;
	newXpos = gridPositionX(getSpriteLocationX(label));
	newYpos = gridPositionY(getSpriteLocationY(label));
	struct Sprite_t current = getSprite(Index);
	if(*drawX != newXpos || *drawY != newYpos){
		while(!(updateSprite)){}
			if(* drawX == newXpos){
				if(* drawY > newYpos) * drawY -= 1;
				else * drawY += 1;
			}
			else if(* drawY == newYpos){
				if(* drawX > newXpos) * drawX -= 1;
				else * drawX += 1;
			}
			reDrawTile(* prevXPosition, * prevYPosition, * drawX, *drawY, current.width, current.height, direction);
			drawSprite(Index, label, * drawX, * drawY, direction, * spriteAnimationCounter);
			updateSprite = 0;
			* spriteAnimationCounter = (* spriteAnimationCounter + 1)&0x0F;
	}
	else{
		* isMoving = 0;
		* prevXPosition = getSpriteLocationX (label);
		* prevYPosition = getSpriteLocationY (label);
	}
	TIMER0_CTL_R &= 0;	
}
void spriteAttack(uint8_t Index, uint8_t label, int16_t drawX, int16_t drawY, uint8_t direction, 
									uint8_t attackAnimationCounter, uint8_t spritesMoving){
	spriteAttacking = 1;
	setTimer(spritesMoving);
	while(!(updateSprite)){}
	updateSprite = 0;
	TIMER0_CTL_R &= 0;
		
	uint8_t xpos = getSpriteLocationX(label);
	uint8_t ypos = getSpriteLocationY(label);
	if(!(((xpos == 0) && (direction == 0)) || ((ypos == 0) && (direction == 1)) || 
		 ((xpos == 7) && (direction == 2)) || ((ypos == 9) && (direction == 3))))
	drawSprite(Index, label, drawX, drawY, direction, attackAnimationCounter);
	if(attackAnimationCounter == 8)	{
		if(!(((xpos == 0) && (direction == 0)) || ((ypos == 0) && (direction == 1)) || 
		 ((xpos == 7) && (direction == 2)) || ((ypos == 9) && (direction == 3)))){
			if(direction == 0) drawTile(getSpriteLocationX(label) - 1, getSpriteLocationY(label));
			else if(direction == 1) drawTile(getSpriteLocationX(label), getSpriteLocationY(label) - 1);
			else if(direction == 2) drawTile(getSpriteLocationX(label) + 1, getSpriteLocationY(label));
			else drawTile(getSpriteLocationX(label), getSpriteLocationY(label) + 1);
			drawTile (getSpriteLocationX(label), getSpriteLocationY(label));
			spriteAttacking = 0;
			drawSprite(Index, label, drawX, drawY, direction, attackAnimationCounter);
		 }
	} 
	spriteAttacking = 0;
}
void cleanSprite (uint8_t Index, int16_t drawX, int16_t drawY){
	struct Sprite_t current = getSprite(Index);
	int16_t lowerX, upperX, upperY, lowerY;
	lowerX = drawX - 4;
	upperX = drawX + current.width + 5;

	lowerY = drawY - (current.height - 1) - 5;
	upperY = drawY + 6;
	
	if(upperY > 160) upperY = 160;
	if(lowerX < 8) lowerX = 8;
	if(upperX > 128) upperX = 128;
	CleanRect(lowerX, upperY, upperX-lowerX, upperY-lowerY);
}
void cleanBox (uint8_t Index, int16_t drawX, int16_t drawY){
	struct Sprite_t current = getSprite(Index);
	int16_t lowerX, upperX, upperY, lowerY;
	lowerX = drawX - 4;
	upperX = drawX + current.width + 5;

	lowerY = drawY - (current.height - 1) - 5;
	upperY = drawY + 6;
	if(upperY > 160) upperY = 160;
	if(lowerX < 8) lowerX = 8;
	if(upperX > 128) upperX = 128;
	CleanRect(lowerX, upperY, upperX-lowerX, upperY-lowerY);
}

void moveSprite(void){
	updateSprite = 1;
}
void setTimer(uint8_t spritesMoving){
	if(spritesMoving == 0) spritesMoving = 1;
	uint16_t divisor = spritesMoving * 40 * 2;
	Timer0_Init(&moveSprite, 80000000/divisor);
}
uint8_t checkEnemyAttacked (uint8_t direction, int16_t drawX, int16_t drawY, int16_t EdrawX, int16_t EdrawY, uint8_t EIndex){
		struct Sprite_t Ecurrent = getSprite(EIndex);
		struct Sprite_t link = getSprite(0);
		int16_t lowerX, upperX, upperY, lowerY, ElowerX, EupperX, EupperY, ElowerY;
		ElowerX = EdrawX + 1;
		EupperX = EdrawX + Ecurrent.width;

		ElowerY = EdrawY - Ecurrent.height;
		EupperY = EdrawY;
	if(direction == 0){
		lowerX = drawX - (link.attackWidth - link.width);
		upperX = drawX;
		
		lowerY = drawY - (link.height - 1);
		upperY = drawY;
		
	}
	else if(direction == 1){
		lowerX = drawX + 1;
		upperX = drawX + link.width;
		
		lowerY = drawY - (link.attackWidth - 1);
		upperY = drawY - (link.height - 1);

	}
	else if(direction == 2){
		lowerX = drawX + link.width;
		upperX = drawX + link.attackWidth;
		
		lowerY = drawY - (link.height - 1);
		upperY = drawY + 1;
	}
	else {
		lowerX = drawX + 1;
		upperX = drawX + link.width;
		
		lowerY = drawY;
		upperY = drawY + (link.attackWidth - link.width);
	}
	if(((lowerX >= ElowerX && lowerX <= EupperX)||(upperX >= ElowerX && upperX <= EupperX))&&
		((lowerY >= ElowerY && lowerY <= EupperY)||(upperY >= ElowerY && upperY <= EupperY)))
			 return 1;		
	return 0;
}
uint8_t checkAttacked (uint8_t direction, int16_t drawX, int16_t drawY, int16_t EdrawX, int16_t EdrawY, uint8_t EIndex){
		struct Sprite_t Ecurrent = getSprite(EIndex);
		struct Sprite_t link = getSprite(0);
		int16_t lowerX, upperX, upperY, lowerY, ElowerX, EupperX, EupperY, ElowerY;
		ElowerX = EdrawX + 1;
		EupperX = EdrawX + Ecurrent.width;

		ElowerY = EdrawY - Ecurrent.height;
		EupperY = EdrawY;
	
		lowerX = drawX + 1;
		upperX = drawX + link.width;

		lowerY = drawY - (link.height - 1);
		upperY = drawY;

	if(((lowerX >= ElowerX && lowerX <= EupperX)||(upperX >= ElowerX && upperX <= EupperX))&&
		((lowerY >= ElowerY && lowerY <= EupperY)||(upperY >= ElowerY && upperY <= EupperY)))
			 return 1;		
	return 0;
}
uint8_t checkBombAttacked (uint8_t xPosition, uint8_t yPosition, int16_t EdrawX, int16_t EdrawY, uint8_t EIndex){
	struct Sprite_t Ecurrent = getSprite(EIndex);
	int16_t lowerX, upperX, upperY, lowerY, ElowerX, EupperX, EupperY, ElowerY;
	ElowerX = EdrawX + 1;
	EupperX = EdrawX + Ecurrent.width;

	ElowerY = EdrawY - Ecurrent.height;
	EupperY = EdrawY;
	
	lowerX = gridPositionX(xPosition);
	upperX = lowerX + 16 + 16;
	lowerX -= 16;
	
	upperY = gridPositionY(yPosition);
	lowerY = upperY - 16 - 16;
	upperY += 16;
	
	//ST7735_FillRect(lowerX, lowerY, upperX-lowerX, upperY-lowerY, 0xFFFF);
	if(EIndex == 4){
		if(((lowerX >= ElowerX && lowerX <= EupperX)||(upperX >= ElowerX && upperX <= EupperX))&&
		   ((lowerY >= ElowerY && lowerY <= EupperY)||(upperY >= ElowerY && upperY <= EupperY))) return 1;
	}
	else{
		if(((lowerX <= ElowerX && ElowerX <= upperX)||(lowerX <= EupperX && EupperX <= upperX))&&
		   ((lowerY <= ElowerY && ElowerY <= upperY)||(upperY <= ElowerY && EupperY <= upperY)))
			 return 1;		
	}
	return 0;
}
void setSpriteAnimation(uint8_t Index, uint8_t direction, uint8_t spriteAnimationCounter){
	struct Sprite_t current = getSprite(Index);
	uint8_t spriteAnimation;
	if(spriteAttacking){
		if(spriteAnimationCounter < 2){
			if(direction == 0)
				current.currentCharacter = current.AttackN;
			else if(direction == 1)
				current.currentCharacter = current.AttackE;
			else if(direction == 2)
				current.currentCharacter = current.AttackS;
			else 
				current.currentCharacter = current.AttackW;
		}
		else if(spriteAnimationCounter < 4){
			if(direction == 0)
				current.currentCharacter = current.AttackNAni1;
			else if(direction == 1)
				current.currentCharacter = current.AttackEAni1;
			else if(direction == 2)
				current.currentCharacter = current.AttackSAni1;
			else 
				current.currentCharacter = current.AttackWAni1;
		}
		else {
			if(direction == 0)
				current.currentCharacter = current.AttackNAni2;
			else if(direction == 1)
				current.currentCharacter = current.AttackEAni2;
			else if(direction == 2)
				current.currentCharacter = current.AttackSAni2;
			else 
				current.currentCharacter = current.AttackWAni2;
		}	
	}
	else{
		if(spriteAnimationCounter >= 8){
			if(spriteAnimation) spriteAnimation = 0;
			else spriteAnimation = 1;
		}
		if(spriteAnimation){
			if(direction == 0)
				current.currentCharacter = current.CharacterNA;
			else if (direction == 1)
				current.currentCharacter = current.CharacterEA;
			else if (direction == 2)
				current.currentCharacter = current.CharacterSA;
			else
				current.currentCharacter = current.CharacterWA;
			}
		else{
			if(direction == 0)
				current.currentCharacter = current.CharacterN;
			else if (direction == 1)
				current.currentCharacter = current.CharacterE;
			else if (direction == 2)
				current.currentCharacter = current.CharacterS;
			else
				current.currentCharacter = current.CharacterW;
		}
	}
}
int16_t gridPositionX (uint8_t Index){
	return (int16_t)((Index * 16) + 8);
}
int16_t gridPositionY (uint8_t Index){
	return (int16_t)((Index * 16) + 15);
}
