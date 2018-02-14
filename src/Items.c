
#include <stdint.h>
#include "../inc/Items.h"
#include "../inc/Sprites.h"
#include "../inc/Grid.h"
#include "../inc/ST7735.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Sentence.h"
#include "../inc/Sprite_Images.h"

struct Item_t{
	uint8_t width;
	uint8_t height;
	const unsigned short * ItemPic;
};

// first item has index 11
void drawItem (uint8_t Index, int16_t drawX, int16_t drawY){
	struct Item_t Item = getItem(Index);
	//change this to exact position instead of grid position 
	SpriteDraw(drawX + 5, drawY - 5, Item.ItemPic, Item.width, Item.height);
}
void cleanItem (uint8_t Index, uint8_t xPosition, uint8_t yPosition){
	struct Item_t Item = getItem(Index);
	int16_t lowerX, upperX, upperY, lowerY;
	int16_t drawX = gridPositionX(xPosition);
	int16_t drawY = gridPositionY(yPosition);
	
	lowerX = drawX - 4 + 5;
	upperX = drawX + Item.width + 5 + 5;

	lowerY = drawY - (Item.height - 1) - 5 - 5;
	upperY = drawY + 6 - 5;
	if(upperY >= 160) upperY = 159;
	if(lowerX < 8) lowerX = 8;
	if(upperX > 128) upperX = 128;
	CleanRect(lowerX, upperY, upperX-lowerX, upperY-lowerY);
}
void drawSelection (uint8_t TriforcePieces, uint8_t BombAmount, uint8_t PotionAmount){
	struct Item_t triUp = getItem(2);
	struct Item_t triDown = getItem(2);
	uint8_t triforcePt;
	int16_t drawX, drawY;
	if(TriforcePieces > 12){TriforcePieces = 12;}
		for(triforcePt = 0; triforcePt < TriforcePieces; triforcePt++){
			drawX = 78; drawY = 57;
			if(triforcePt < 4) {drawY -= 5 * triforcePt;
				ST7735_DrawBitmap(drawX, drawY, triUp.ItemPic, triUp.width, triUp.height);
			}
			else if(triforcePt < 6) {drawY -= 3 + (10 * (triforcePt - 4));
				ST7735_DrawBitmap(drawX, drawY, triDown.ItemPic, triDown.width, triDown.height);
			}
			else if(triforcePt < 8) {drawX -= 5; drawY -= 3 + (10 * (triforcePt - 6));
				ST7735_DrawBitmap(drawX, drawY, triUp.ItemPic, triUp.width, triUp.height);
			}
			else if(triforcePt < 10) {drawX -= 10; drawY -= 5 + (5 * (triforcePt - 8));
				ST7735_DrawBitmap(drawX, drawY, triUp.ItemPic, triUp.width, triUp.height);
			}
			else if(triforcePt < 11) {drawX -= 10; drawY -= 8 + (triforcePt - 10);
				ST7735_DrawBitmap(drawX, drawY, triDown.ItemPic, triDown.width, triDown.height);
			}
			else{drawX -= 15; drawY -= 8 + (triforcePt - 11);
				ST7735_DrawBitmap(drawX, drawY, triUp.ItemPic, triUp.width, triUp.height);
			}
		
	}
	uint8_t digits;
	struct Item_t blackBomb = getItem(7);
	drawY = 113; drawX = 62;
	ST7735_DrawBitmap(drawX, drawY, blackBomb.ItemPic, blackBomb.width, blackBomb.height);
	if(BombAmount < 10) digits = 1;
	else if(BombAmount < 100) digits = 2;
	drawX += blackBomb.width + 3; // bow is 12 by 6
	drawNumber (BombAmount, digits, drawX, drawY);
	
	struct Item_t blackPotion = getItem(8);
	drawY = 90; drawX = 62;
	ST7735_DrawBitmap(drawX, drawY-2, blackPotion.ItemPic, blackPotion.width, blackPotion.height);
	if(PotionAmount < 10) digits = 1;
	else if(PotionAmount < 100) digits = 2;
	drawX += blackPotion.width + 3; // bow is 12 by 6
	drawNumber (PotionAmount, digits, drawX, drawY);
}
void drawCloud (uint8_t xPosition, uint8_t yPosition, uint8_t cloudCountdown){
	struct Item_t bombCloud = getItem(6);
	int16_t lowerX, upperX, upperY, lowerY;
	lowerX = gridPositionX(xPosition);
	upperX = lowerX + 16 + 16;
	lowerX -= 16;
	
	upperY = gridPositionY(yPosition);
	lowerY = upperY - 16 - 16;
	upperY += 16;
	
	if(lowerX <= 8) {lowerX = 8; upperX += 16; }
	if(upperY >= 160) upperY = 159;
	
	if(cloudCountdown > 0){
		SpriteDraw(lowerX+6, upperY-6, bombCloud.ItemPic, bombCloud.width, bombCloud.height);
		SpriteDraw(lowerX+16, upperY-16, bombCloud.ItemPic, bombCloud.width, bombCloud.height);
		SpriteDraw(lowerX+26, upperY-6, bombCloud.ItemPic, bombCloud.width, bombCloud.height);
	}
	else
		CleanRect(lowerX, upperY, upperX-lowerX, upperY-lowerY);
	
}
void drawDoorItems (uint8_t swordActive, uint8_t fireAnimationCounter){
	struct Item_t fire = getItem(9);
	struct Item_t fireAni = getItem(10);
	struct Item_t oldMan = getItem(11);
	struct Item_t magicalSword = getItem(12);
	if(fireAnimationCounter < 8){
		SpriteDraw(gridPositionX (2), gridPositionY (7), fire.ItemPic, fire.width, fire.height);
		SpriteDraw(gridPositionX (2), gridPositionY (2), fireAni.ItemPic, fireAni.width, fireAni.height);
	}
	else{
		SpriteDraw(gridPositionX (2), gridPositionY (7), fireAni.ItemPic, fireAni.width, fireAni.height);
		SpriteDraw(gridPositionX (2), gridPositionY (2), fire.ItemPic, fire.width, fire.height);
	}
	SpriteDraw(gridPositionX (2), gridPositionY (4), oldMan.ItemPic, oldMan.width, oldMan.height);
	if(!(swordActive)) {
		SpriteDraw(gridPositionX (2), gridPositionY (5), magicalSword.ItemPic, magicalSword.width, magicalSword.height);
	}
	drawSentence(("YOU MASTERED EE"), 15, gridPositionX (1) + 8, centerY(15) + 13);
	drawNumber(319, 3, gridPositionX (1) + 8, 48);
	drawSentence(("K"), 1, gridPositionX (1) + 8, 28);
	
	drawSentence(("TAKE THIS"), 9, gridPositionX (3) + 4, centerY(9));
}
void drawFinalRoom (void){
	struct Item_t zelda = getItem(13);
	SpriteDraw(gridPositionX (3), gridPositionY (4) - 3, zelda.ItemPic, zelda.width, zelda.height);
}
