
#include <stdint.h>
#include "Grid.h"
#include "Sprites.h"
#include "tm4c123gh6pm.h"
#include "ST7735.h"

uint8_t MapGrid [10][8];
uint8_t LinkGrid [10][8];
uint8_t EnemyGrid [10][8];
uint8_t ItemGrid [10][8];

uint8_t xpos;
uint8_t ypos;

void setGrid(uint8_t Map[10][8]){
	uint8_t row;
	for(row = 0; row < 10; row ++){
		uint8_t col;
		for(col = 0; col < 8; col++){
			if(Map[row][col] != 4 && Map[row][col] != 8 && Map[row][col] != 24 && 
				 Map[row][col] != 30 && Map[row][col] != 3){
				MapGrid[row][col] = 1;
				LinkGrid[row][col] = 1;
				EnemyGrid[row][col] = 1;
			}
			else{
				if(Map[row][col] != 3){
					MapGrid[row][col] = 0;
					LinkGrid[row][col] = 0;
				}
				else{
					MapGrid[row][col] = 2;
					LinkGrid[row][col] = 2;
				}
				
				EnemyGrid[row][col] = 0;
			}
			ItemGrid[row][col] = 0;
		}
	}
}
void showGrid (void){
	xpos = 0; ypos = 0;
	uint8_t row;
	for(row = 0; row < 10; row ++){
		uint8_t col;
		for(col = 0; col < 8; col++){
			ST7735_SetCursor(xpos, ypos);
			LCD_OutDec(MapGrid[row][col]);
			xpos += 1;
		}
		xpos = 0;
		ypos += 1;
	}
	xpos = 0; ypos = 0;
}
void setGridPosition (uint8_t label, uint8_t xPosition, uint8_t yPosition){
	if(label == 3){
		LinkGrid[yPosition][xPosition] = label;
		MapGrid[yPosition][xPosition] = label + ItemGrid[yPosition][xPosition] + EnemyGrid[yPosition][xPosition];
	}
	else if(label < 11){
		EnemyGrid[yPosition][xPosition] = label;
		MapGrid[yPosition][xPosition] = label + ItemGrid[yPosition][xPosition] + LinkGrid[yPosition][xPosition];
	}
	else{
		ItemGrid[yPosition][xPosition] = label;
		MapGrid[yPosition][xPosition] = label + EnemyGrid[yPosition][xPosition] + LinkGrid[yPosition][xPosition];
	}
}
void clearGridPosition (uint8_t label, uint8_t xPosition, uint8_t yPosition){
		if(label == 3){
			LinkGrid[yPosition][xPosition] = 0;
			MapGrid[yPosition][xPosition] = ItemGrid[yPosition][xPosition] + EnemyGrid[yPosition][xPosition];
		}
		else if(label < 11){
			EnemyGrid[yPosition][xPosition] = 0;
			MapGrid[yPosition][xPosition] = ItemGrid[yPosition][xPosition] + LinkGrid[yPosition][xPosition];
		}
		else{
			ItemGrid[yPosition][xPosition] = 0;
			MapGrid[yPosition][xPosition] = EnemyGrid[yPosition][xPosition] + LinkGrid[yPosition][xPosition];
		}
}
uint8_t getSpriteLocationX(uint8_t label){
	uint8_t row;
	for(row = 0; row < 10; row ++){
		uint8_t col;
		for(col = 0; col < 8; col++){
			if(label == 3){
				if(LinkGrid[row][col] == label) return col;}
			else if(label < 11){
				if(EnemyGrid[row][col] == label) return col;}
			else {
				if(ItemGrid[row][col] == label) return col;}
		}
	}
	return 50;
}
uint8_t getSpriteLocationY(uint8_t label){
	uint8_t row;
	for(row = 0; row < 10; row ++){
		uint8_t col;
		for(col = 0; col < 8; col++){
			if(label == 3){
				if(LinkGrid[row][col] == label) return row;}
			else if(label < 11){
				if(EnemyGrid[row][col] == label) return row;}
			else{
				if(ItemGrid[row][col] == label) return row;}
		}
	}
	return 50;
}
uint8_t isLocationClear(uint8_t xPosition, uint8_t yPosition){
	if(MapGrid[yPosition][xPosition] == 0) return 1;
	else return 0;
}
uint8_t checkMove (uint8_t moveDirection, uint8_t label){
	int8_t rowAddition = 0;
	int8_t colAddition = 0;
	if(moveDirection == 0) colAddition = -1;
	else if(moveDirection == 1) rowAddition = -1;
	else if(moveDirection == 2) colAddition = 1;
	else if(moveDirection == 3) rowAddition = 1;
	rowAddition += getSpriteLocationY(label);
	colAddition += getSpriteLocationX(label);
	if(rowAddition < 0 || rowAddition >= 10 ||
		colAddition < 0 || colAddition >= 8) return 0;
	if(label == 3){
		if((LinkGrid[rowAddition][colAddition] != 1) && (MapGrid[rowAddition][colAddition] != 2)) return 1;
	}
	else if(label < 11){
		if((EnemyGrid[rowAddition][colAddition] == 0) && colAddition != 0) return 1;
	}
	else{
		if(ItemGrid[rowAddition][colAddition] == 0 && colAddition != 0) return 1;
	}
	return 0;
}
uint8_t spriteMove (uint8_t label, uint8_t direction){
	xpos = getSpriteLocationX(label);
	ypos = getSpriteLocationY(label);
	if(checkMove(direction, label)) {	
		if(direction == 0) setGridPosition(label, xpos - 1, ypos);
		else if (direction == 1) setGridPosition(label, xpos, ypos - 1);
		else if (direction == 2) setGridPosition(label, xpos + 1, ypos);
		else if (direction == 3) setGridPosition(label, xpos, ypos + 1);
		clearGridPosition(label, xpos, ypos);
	}
	else
		if((((xpos == 0) && (direction == 0)) || ((xpos == 7) && (direction == 2)) || 
			 ((ypos == 0) && (direction == 1)) || ((ypos == 9) && (direction == 3)) || (LinkGrid[ypos][xpos - 1] == 2)) && (label == 3)) return 1; 
	return 0;
}
uint8_t getNextMap (void){
	xpos = getSpriteLocationX(3); // col
	ypos = getSpriteLocationY(3); // row
	if(xpos == 0) return 0;
	else if(ypos == 0) return 1;
	else if(xpos == 7) return 2;
	else if(ypos == 9) return 3;
	else return 4;
}
uint8_t itemPickedUp (void){
	xpos = getSpriteLocationX(3); // col
	ypos = getSpriteLocationY(3); // row
	if(ItemGrid[ypos][xpos] >= 11) {
		uint8_t pickedItem = ItemGrid[ypos][xpos];
		clearGridPosition(pickedItem, xpos, ypos);
		return pickedItem;
	}
	return 0;
}
uint8_t canPlaceItem(uint8_t direction){
	xpos = getSpriteLocationX(3); // col
	ypos = getSpriteLocationY(3); // row
	if(((xpos == 0) && (direction == 0)) || ((xpos == 7) && (direction == 2)) || 
	 ((ypos == 0) && (direction == 1)) || ((ypos == 9) && (direction == 3))) return 0; 
	return 1;
}
void placeItem(uint8_t label, uint8_t direction){
	xpos = getSpriteLocationX(3); // col
	ypos = getSpriteLocationY(3); // row
	if(direction == 0) setGridPosition(label, xpos - 1, ypos);
	else if (direction == 1) setGridPosition(label, xpos, ypos - 1);
	else if (direction == 2) setGridPosition(label, xpos + 1, ypos);
	else if (direction == 3) setGridPosition(label, xpos, ypos + 1);
}
void clearItemGrid(void){
	uint8_t row;
	for(row = 0; row < 10; row ++){
		uint8_t col;
		for(col = 0; col < 8; col++){
			ItemGrid[row][col] = 0;
		}
	}
}

