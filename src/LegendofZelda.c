// Legend of Zelda.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)

// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "../inc/Random.h"
#include "../inc/TExaS.h"
#include "../inc/ADC.h"
#include "../inc/Map.h"
#include "../inc/SysTickInts.h"
#include "../inc/Sprites.h"
#include "../inc/Grid.h"
#include "../inc/Items.h"
#include "../inc/DAC.h"
#include "../inc/Sound.h"
#include "../inc/Sentence.h"
#include "../inc/Sprite_Images.h"
#include "../inc/I_O.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

#define North 0								
#define East 1
#define South 2
#define West 3

#define TRUE 1
#define FALSE 0

static uint8_t mapPt = 0;					 	// index of current map; Start Location = 0

static uint8_t updateMap = 0;				// update map enable/disable

static uint8_t nextMap = 0;					// index of next map
//static uint8_t updateSprites = 0;		// update sprites enable/disable
static uint8_t updateHeart = 1;			// update hearts display enable/disable
static uint8_t selectScreen = 0;			// selection screen enable/disable
static uint8_t selectDown = 0;				// is selcetion screen displayed?
static uint8_t selectUp = 1;					// is selection screen not displayed?
static uint8_t selectChange = 0;			// does the selection screen need to be removed?
static uint8_t updateSelect = 0;			// does the selection screen need to be displayed?

static uint8_t attackDown = 0;				// is attack button pressed?
static uint8_t attackUp = 1;					// is attack button not pressed?
static int8_t characterPt = 0;				// index of current character
static uint8_t numCharacters = 1;		// number of characters present
static uint8_t numSpritesMoving = 0;	// number of sprites moving
static uint8_t invicCounter = 0;			// countdown to amount of invincibility left
static uint8_t moveCounter = 0;			// moves link up 4 at the end of the game; possible to remove with changes

static uint8_t itemPt = 0;						// index of current item selected; possible to remove with changes
//static uint8_t itemDropped = 0;			// index of type of item dropped
static uint8_t updateItems = 0;			// id of item picked up; 0 if no item is picked up
static uint8_t triForcePieces = 0;		// number of triforce pieces collected
static uint8_t openBoss = 0;					// should the boss door be opened?

static uint8_t activeItem = 0;				// current active item selected
static uint8_t useItem = 0;					// is item used?
static uint8_t bombCountdown = 0;		// countdown delay between bombs
static uint8_t bombCountingDown = 0;	// is bomb counting down?
static uint8_t PotionCountDown = 0;	// countdown delay between potions
static uint8_t cloudCountdown = 0;		// countdown delay between clouds
static uint8_t cloudCountingDown = 0;// is cloud counting down?
//static uint8_t itemCoolDown = 0;			// not used but maybe use?

static int32_t score = 0;						// user score that is displayed at top left of screen
static uint8_t updateScore = 1;			// does the score need to be updated?

static uint8_t swordActive = 0;			// is sword picked up?
static uint8_t fireAnimationCounter = 0; // counter for
static uint8_t bossAlive = 1;				// is final boss alive?
static uint8_t deathScreen = 1;			// death screen enabled; need to change for logical concerns
static uint8_t completeScreen = 0;		// is completion screen shown?

static uint8_t completeTaskOnce = 1;	// needed to move link in final screen; possible to remove with changes
static uint8_t gameFinished = 0;			// is game finished?
static uint8_t invincibility = 1; 		// is invincibility activated? 
/********** Character Struct**********/
struct Character_t{
	uint8_t sprite;											// type of sprite image to be drawn
	uint8_t label;											// indicator of character's number on grid
	uint8_t state;											// 0 is dead 1 is alive
	uint8_t direction;									// direction facing; 0 - N, 1 - E, 2 - S, 3 - W
	uint8_t xpos;												// current X position on grid; can be potentially removed
	uint8_t ypos;												// current Y position on grid; can be potentially removed
	int16_t drawX;											// start x 	index of where to draw
	int16_t drawY;											// start y index of where to draw
	
	int8_t life;												// lifepoints left
	uint8_t moveDirection;							// direction moving
	uint8_t isMoving;										// is the character moving?
	uint8_t spriteAnimationCounter;			// number of animations left to move
	uint8_t attackAnimationCounter;			// number of attack animations left
	uint8_t speedCounter;								// character movement speed
	uint8_t speedDivider;								// how many times 
};
struct Character_t Characters [8] = {{
0, 3, 1, South, 0, 0, 0, 0, 10, 4, 0, 0, 0, 0, 1 //Link example
}};
/********** Active Items Struct**********/
struct ActiveItems_t{
	uint8_t active;
	uint8_t display;
	uint8_t type;
	uint8_t label;
	uint8_t xpos;
	uint8_t ypos;
};
struct ActiveItems_t ActiveItems [20] = {
{0, 0, 0, 11, 0, 0} // TriForce Example
};
/********** Usable Items Struct**********/
struct UseItems_t{
	uint8_t active;
	uint8_t display;
	uint8_t amount;
	uint8_t type;
	uint8_t xpos;
	uint8_t ypos;
};
struct UseItems_t UseItem [2] = {
{0, 0, 10, 4, 0, 0}, 
{0, 0, 5, 5, 0, 0} 
};
/********** World Map Struct**********/
struct WorldMap_t{
	uint8_t MapClear;
	uint8_t EnemyCount;
	uint8_t EnemyType[8];
	int8_t NextMap[5]; // 0 index is N, 1 is E, 2 is S, 3 is W, 4 is Door
};


struct WorldMap_t WorldMap [18] = {
	 // default not reachable screen edge is -1
	 // default not used door is -1
{1, 0, {0, 0, 0, 0, 0, 0, 0, 0}, { 1, 2,-1, 3,15}}, //0  
{0, 2, {1, 1, 1, 1, 0, 0, 0, 0}, {-1, 4, 0, 6,-1}}, //1
{0, 3, {2, 2, 2, 1, 0, 0, 0, 0}, { 4, 7,-1, 0,-1}}, //2
{0, 4, {1, 1, 1, 2, 0, 0, 0, 0}, { 6, 0,-1, 8,-1}}, //3
{0, 4, {2, 2, 2, 2, 0, 0, 0, 0}, { 5, 9, 2, 1,-1}}, //4
{0, 4, {1, 2, 2, 3, 0, 0, 0, 0}, {-1,-1, 4,-1,-1}}, //5
{0, 3, {1, 2, 2, 2, 2, 0, 0, 0}, {12, 1, 3,10,-1}}, //6
{0, 2, {2, 2, 0, 0, 0, 0, 0, 0}, { 9,-1, 7, 2,-1}}, //7
{0, 3, {1, 2, 2, 0, 0, 0, 0, 0}, {10, 3,11,-1,-1}}, //8
{0, 3, {3, 3, 3, 3, 0, 0, 0, 0}, {14,-1, 7, 4,-1}}, //9
{0, 3, {2, 2, 2, 3, 3, 0, 0, 0}, {-1, 6, 8,-1,-1}},	//10
{0, 3, {2, 2, 2, 3, 3, 0, 0, 0}, { 8,-1,-1,-1,-1}},	//11
{0, 3, {3, 2, 2, 3, 3, 0, 0, 0}, {-1,-1, 6,-1,-1}},	//12 Extra map that needs to be added :c
{1, 0, {2, 2, 2, 3, 3, 0, 0, 0}, {14,-1, 7, 4,-1}},	//13 Updated map when 12 pieces collected
{1, 0, {2, 2, 2, 3, 3, 0, 0, 0}, {-1,-1,13,-1,16}},	//14 Boss door when 12 pieces collected
{1, 0, {2, 2, 2, 3, 3, 0, 0, 0}, {-1,-1, 0,-1,-1}},	//15 Pick up sword area
{0, 7, {4, 3, 3, 3, 3, 3, 3, 3}, {17,-1,14,-1,-1}},	//16 Final boss room
{1, 0, {2, 2, 2, 3, 3, 0, 0, 0}, {-1,-1,16,-1,-1}}	//17 Final room
// Start of Dungeon. Too bad it's only 1 map :C

};
void Port_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x33;												// Turns on clock for Port A, B, E, F
	while((SYSCTL_PRGPIO_R&0x01)==0){};
	// initializes port E for input
	GPIO_PORTE_DIR_R &= ~0x2F;												
	GPIO_PORTE_DEN_R |=	 0x2F;				
	GPIO_PORTE_AFSEL_R &= ~0x2F;
	GPIO_PORTE_AMSEL_R &= ~0x2F;
	GPIO_PORTE_PCTL_R &= ~0x2F;
	// initializes port F input
	GPIO_PORTF_DIR_R &= ~0x10;
	GPIO_PORTF_DEN_R |=	 0x10;
	GPIO_PORTF_AFSEL_R &= ~0x10;
	GPIO_PORTF_AMSEL_R &= ~0x10;
	GPIO_PORTF_PUR_R |= 0x10;
	GPIO_PORTF_PDR_R |= 0x01;
}
void Generate_Sprite_Random(void){
	uint8_t setXpos = Random()%8;
						uint8_t setYpos = Random()%10;
						while(!(isLocationClear(setXpos, setYpos))){
							setXpos = Random()%8;
							setYpos = Random()%10;
						}
						Characters[characterPt].sprite = WorldMap[mapPt].EnemyType[characterPt - 1];
						Characters[characterPt].state = 1;
						Characters[characterPt].label = characterPt + 3;
						Characters[characterPt].direction = Random()%4;
						Characters[characterPt].xpos = setXpos;
						Characters[characterPt].ypos = setYpos;
						Characters[characterPt].drawX = gridPositionX (setXpos);
						Characters[characterPt].drawY = gridPositionY (setYpos);
						
						Characters[characterPt].life = WorldMap[mapPt].EnemyType[characterPt - 1];
						Characters[characterPt].moveDirection = 4;
						Characters[characterPt].isMoving = 0;
						Characters[characterPt].spriteAnimationCounter = 0;
						Characters[characterPt].attackAnimationCounter = 0;
						Characters[characterPt].speedCounter = 0;
						Characters[characterPt].speedDivider = 4;
						
						spriteInit(Characters[characterPt].sprite, Characters[characterPt].xpos, Characters[characterPt].ypos,
						Characters[characterPt].direction , Characters[characterPt].label);
}
void update_Map(void){
	updateMap = 0;
				uint8_t leavingDoor, bossDoor, finalDoor;
				leavingDoor = 0; bossDoor = 0; finalDoor = 0;
				if(mapPt == 15) leavingDoor = 1;
				
				drawMap(WorldMap[mapPt].NextMap[nextMap]);
				mapPt = WorldMap[mapPt].NextMap[nextMap];
				
				if(mapPt == 16) bossDoor = 1;
				else if(mapPt == 17) finalDoor = 1;
					
				if(leavingDoor){
					Characters[0].xpos = 1;
					Characters[0].ypos = 7;
				}
				else if(bossDoor){
					Play_Song(DUNGEON);
					Characters[0].xpos = 6;
				}
				else if(finalDoor){
					Play_Song(INTRO);
					Characters[0].ypos = 5;
					Characters[0].xpos = 7;
				}
				else if(nextMap == 0)
					Characters[0].xpos = 7; 
				else if(nextMap == 1)
					Characters[0].ypos = 9; 
				else if(nextMap == 2)
					Characters[0].xpos = 0; 
				else if(nextMap == 3)
					Characters[0].ypos = 0;
				else if(nextMap == 4){
					Characters[0].xpos = 7;
					Characters[0].ypos = 4;
				}
				
			
				
				Characters[0].drawX = gridPositionX (Characters[0].xpos);
				Characters[0].drawY = gridPositionY (Characters[0].ypos);
				spriteInit(Characters[0].sprite, Characters[0].xpos, Characters[0].ypos,
				Characters[0].direction , Characters[0].label);
				
				if(!(WorldMap[mapPt].MapClear)){
					numCharacters = WorldMap[mapPt].EnemyCount + 1;
					for(characterPt = 1; characterPt < numCharacters; characterPt++){
						// Draws sets up normal enemies to the map
						if(WorldMap[mapPt].EnemyType[characterPt - 1] != 4){ Generate_Sprite_Random(); }
						else{ // Draws Ganon to the map
							Characters[characterPt].sprite = WorldMap[mapPt].EnemyType[characterPt - 1];
							Characters[characterPt].state = 1;
							Characters[characterPt].label = characterPt + 3;
							Characters[characterPt].direction = North;
							Characters[characterPt].xpos = 0;
							Characters[characterPt].ypos = 5;
							Characters[characterPt].drawX = gridPositionX (0);
							Characters[characterPt].drawY = gridPositionY (5);
							
							Characters[characterPt].life = 10;
							Characters[characterPt].moveDirection = 4;
							Characters[characterPt].isMoving = 0;
							Characters[characterPt].spriteAnimationCounter = 0;
							Characters[characterPt].attackAnimationCounter = 0;
							
							spriteInit(Characters[characterPt].sprite, Characters[characterPt].xpos, Characters[characterPt].ypos,
							Characters[characterPt].direction , Characters[characterPt].label);
						}
					}
					// Triforce pieces collected
					for(itemPt = 0; itemPt < 12; itemPt ++){
						ActiveItems[itemPt].display = 0;
					}
					// Normal items dropped by enemies
					for(itemPt = 12; itemPt < 20; itemPt ++){
						ActiveItems[itemPt].active = 0;
						ActiveItems[itemPt].display = 0;
						ActiveItems[itemPt].type = 0;
						ActiveItems[itemPt].label = itemPt + 11;
						ActiveItems[itemPt].xpos = 0;
						ActiveItems[itemPt].ypos = 0;
					}
					// Items used. Particularly bombs
					for(itemPt = 0; itemPt < 2; itemPt++){
						UseItem[itemPt].display = 0;
					}
					// Clears item grid
					clearItemGrid();
				}
				else{
					numCharacters = 1;
					if(mapPt < 13 && mapPt != 0){
						if(ActiveItems[mapPt-1].active) {
							ActiveItems[mapPt-1].display = 1;
							setGridPosition(ActiveItems[mapPt - 1].label, ActiveItems[mapPt - 1].xpos, ActiveItems[mapPt - 1].ypos);
						}
					}
			}
}
void update_Items(void){
	if(!(swordActive) && updateItems == 21) { // If sword is picked up
		Play_FX(SECRET);
					swordActive = 1;
					CleanRect(gridPositionX (2), gridPositionY (5), 16, 8);
					clearGridPosition(21, 2, 5);
				}
				for(itemPt = 0; itemPt < 20; itemPt++){
					if(ActiveItems[itemPt].active && ActiveItems[itemPt].display && ActiveItems[itemPt].label == updateItems){
						Play_FX(GET_HEART);
						if(ActiveItems[itemPt].type == 0){
							triForcePieces++;
							if(triForcePieces == 12) {
								Play_FX(SECRET);
								openBoss = 1;
							}
						}
						if(ActiveItems[itemPt].type == 1){Characters[0].life++; updateHeart = 1;}
						if(ActiveItems[itemPt].type == 4) UseItem[0].amount ++;
						if(ActiveItems[itemPt].type == 5) UseItem[1].amount ++;
						ActiveItems[itemPt].active = 0; ActiveItems[itemPt].display = 0;
						cleanItem(ActiveItems[itemPt].type, ActiveItems[itemPt].xpos, ActiveItems[itemPt].ypos);
						clearGridPosition (ActiveItems[itemPt].label, ActiveItems[itemPt].xpos, ActiveItems[itemPt].ypos);
						break;
					}
				}
				updateItems = 0;
}
void update_Attack(void){
		if(Characters[0].attackAnimationCounter == 0){
					Play_FX(SWORD_SLASH);
					for(characterPt = 1; characterPt < numCharacters; characterPt++){
						if(Characters[characterPt].state != 0){
						uint8_t enemyAttacked = checkEnemyAttacked(Characters[0].direction, Characters[0].drawX, Characters[0].drawY,
																							Characters[characterPt].drawX, Characters[characterPt].drawY, Characters[characterPt].sprite);
						if(enemyAttacked){
							Play_FX(HIT);
							if(Characters[characterPt].sprite == 4) Play_FX(BOSS_SCREAM);
							Characters[characterPt].life --;
							if((Characters[characterPt].direction == Characters[0].direction) || 
									(Characters[characterPt].direction == ((Characters[0].direction + 2)&0x04)))
									spriteMove(Characters[characterPt].label, Characters[0].direction);
							if(Characters[characterPt].life <= 0){
									Play_FX(KILL);
									updateScore = 1;
									if(mapPt == 16) bossAlive = 0;
									if(Characters[characterPt].sprite != 4)
									score += Characters[characterPt].sprite * 50;
									else score += 2000;
									Characters[characterPt].state = 0;
									Characters[characterPt].isMoving = 0;
									cleanSprite (Characters[characterPt].sprite, Characters[characterPt].drawX, Characters[characterPt].drawY);
									uint8_t tempPt = 1;
									for(tempPt = 1; tempPt < numCharacters; tempPt++){
										if(Characters[tempPt].state != 0) {WorldMap[mapPt].MapClear = 0; break;}
										if(tempPt == (numCharacters - 1)) {
											WorldMap[mapPt].MapClear = 1;
										}
									}
									if(WorldMap[mapPt].MapClear == 1 && mapPt < 13 && mapPt != 0){
										
										ActiveItems[mapPt -1].active = 1;
										ActiveItems[mapPt -1].display = 1;
										ActiveItems[mapPt -1].xpos = Characters[characterPt].xpos;
										ActiveItems[mapPt -1].ypos = Characters[characterPt].ypos;
										setGridPosition(ActiveItems[mapPt - 1].label, ActiveItems[mapPt - 1].xpos, ActiveItems[mapPt - 1].ypos);
									}
									else{
										uint8_t ItemDropped = Random()%10;
										if(ItemDropped < 7){
										for(itemPt = 12; itemPt < 20; itemPt++){
											if(ActiveItems[itemPt].active == 0) {
												ActiveItems[itemPt].active = 1;
												ActiveItems[itemPt].display = 1;
												uint8_t ItemType = Random()%10;
												if(ItemType < 7) ActiveItems[itemPt].type = 1;
												else if (ItemType < 9) ActiveItems[itemPt].type = 4;
												else ActiveItems[itemPt].type = 5;
												ActiveItems[itemPt].xpos = Characters[characterPt].xpos;
												ActiveItems[itemPt].ypos = Characters[characterPt].ypos;
												setGridPosition(ActiveItems[itemPt].label, ActiveItems[itemPt].xpos, ActiveItems[itemPt].ypos);
												break;
											}
										}
									}
								}
								clearGridPosition (Characters[characterPt].label, Characters[characterPt].xpos, Characters[characterPt].ypos);
								updateItems = itemPickedUp();
								break;
							}
						}
					}
				}
			}
				if(Characters[0].attackAnimationCounter < 9){
					spriteAttack (0, Characters[0].label, Characters[0].drawX,
					Characters[0].drawY, Characters[0].direction, Characters[0].attackAnimationCounter,
					numSpritesMoving);
					Characters[0].attackAnimationCounter ++;
				}
				else
					attackDown = 0;
			
}
void update_Sprites(void){
					for(characterPt = 0; characterPt < numCharacters; characterPt++){
					if(!(characterPt == 0 && attackDown)){
						if(!(Characters[characterPt].isMoving) && Characters[characterPt].state){
							if(((Characters[characterPt].direction == Characters[characterPt].moveDirection) || (characterPt != 0)) && (Characters[characterPt].moveDirection == 0 || Characters[characterPt].moveDirection == 1 ||
								Characters[characterPt].moveDirection == 2 || Characters[characterPt].moveDirection == 3)) {
								Characters[characterPt].direction = Characters[characterPt].moveDirection;
								updateMap = spriteMove(Characters[characterPt].label, Characters[characterPt].moveDirection);
								if(updateMap)
									nextMap = getNextMap();
								else{
									if(completeScreen) moveCounter++;
									Characters[characterPt].isMoving = 1;
								}
								if(characterPt == 0)
									updateItems = itemPickedUp();
							}
							else if (Characters[characterPt].moveDirection == 0 || Characters[characterPt].moveDirection == 1 ||
								Characters[characterPt].moveDirection == 2 || Characters[characterPt].moveDirection == 3)
								Characters[characterPt].direction = Characters[characterPt].moveDirection;
							if(Characters[characterPt].life < 6 && Characters[characterPt].sprite == 4) Characters[characterPt].direction = West;
							drawSprite (Characters[characterPt].sprite, Characters[characterPt].label, Characters[characterPt].drawX,
							Characters[characterPt].drawY, Characters[characterPt].direction, Characters[characterPt].spriteAnimationCounter);
						}
					}
				}
			//updateSprites = 0;
}
void update_Hearts(void){
		updateHeart = 0;
				if(Characters[0].life < 0) Characters[0].life = 0;
				if(Characters[0].life > 10) Characters[0].life = 10;
				uint8_t HeartIndex, leftoffset, rightoffset;
				leftoffset = 155; rightoffset = 131;
				
				for(HeartIndex = 0; HeartIndex < Characters[0].life/2; HeartIndex ++){
					drawItem(14, 1, leftoffset);
					leftoffset -= 6;
				}
				for(HeartIndex = 0; HeartIndex < 5 - (Characters[0].life/2); HeartIndex ++){
					drawItem(15, 1, rightoffset);
					rightoffset += 6;
				}
				if(Characters[0].life % 2 != 0){
					drawItem(16, 1, leftoffset);
					leftoffset -= 6;
				}
}
void update_Score(void){
				updateScore = 0;
				uint8_t digits;
				if(score < 0) {score = 0; digits = 1;}
				else if(score < 10) digits = 1;
				else if(score < 100) digits = 2;
				else if(score < 1000) digits = 3;
				else if(score < 10000) digits = 4;
				drawNumber(score, digits, 1, floatRight(digits));
				drawSentence (("SCORE"), 5, 1, floatRight(4) + floatRight(5) + 3);
}

void use_Item(void){
		if(activeItem == 0){
			if(!(bombCountingDown) && canPlaceItem(Characters[0].direction) && UseItem[0].amount > 0){
				UseItem[0].amount--;
				bombCountingDown = 1;
				bombCountdown = 8;
				UseItem[0].display = 1;
					
				placeItem(20, Characters[0].direction);
				UseItem[0].xpos = getSpriteLocationX(20);
				UseItem[0].ypos = getSpriteLocationY(20);
				drawItem(UseItem[0].type, gridPositionX(UseItem[0].xpos), gridPositionY(UseItem[0].ypos));
			}useItem = 0;
		}
		else{ 
			if(PotionCountDown == 0 && UseItem[1].amount > 0){
				PotionCountDown = 8;
				Play_FX(GET_HEART);
				UseItem[1].amount --;
				Characters[0].life += 5;
				if(Characters[0].life > 10) Characters[0].life = 10;
				updateHeart = 1;
			}useItem = 0;
		}
}

void bomb_Physics(void){
			if(cloudCountdown > 0)
				drawCloud(UseItem[0].xpos, UseItem[0].ypos, cloudCountdown);
			else if (cloudCountdown == 0 && cloudCountingDown){
				drawCloud(UseItem[0].xpos, UseItem[0].ypos, cloudCountdown);
				cloudCountingDown = 0;
			}
			else if(bombCountdown == 0 && bombCountingDown){
					Play_FX(BOMB_BLOW);
					UseItem[0].display = 0;
					cleanItem(UseItem[0].type, UseItem[0].xpos, UseItem[0].ypos);
					clearGridPosition (20, UseItem[0].xpos, UseItem[0].ypos);
					for(characterPt = 1; characterPt < numCharacters; characterPt++){
						if(Characters[characterPt].state != 0){
						uint8_t enemyAttacked = checkBombAttacked(UseItem[0].xpos, UseItem[0].ypos,
							Characters[characterPt].drawX, Characters[characterPt].drawY, Characters[characterPt].sprite);
						if(enemyAttacked){
							Characters[characterPt].life -= 3;
							if(Characters[characterPt].sprite == 4) Play_FX(BOSS_SCREAM);
								if(Characters[characterPt].life <= 0){
										Play_FX(KILL);
										updateScore = 1;
										if(mapPt == 16) bossAlive = 0;
										if(Characters[characterPt].sprite != 4)
										score += Characters[characterPt].sprite * 50;
										else score += 2000;
										Characters[characterPt].state = 0;
										Characters[characterPt].isMoving = 0;
										cleanSprite (Characters[characterPt].sprite, Characters[characterPt].drawX, Characters[characterPt].drawY);
										uint8_t tempPt = 1;
										for(tempPt = 1; tempPt < numCharacters; tempPt++){
											if(Characters[tempPt].state != 0) {WorldMap[mapPt].MapClear = 0; break;}
											if(tempPt == (numCharacters - 1)) {
												WorldMap[mapPt].MapClear = 1;
											}
										}
										if(WorldMap[mapPt].MapClear == 1 && mapPt < 13 && mapPt != 0){
											ActiveItems[mapPt -1].active = 1;
											ActiveItems[mapPt -1].display = 1;
											ActiveItems[mapPt -1].xpos = Characters[characterPt].xpos;
											ActiveItems[mapPt -1].ypos = Characters[characterPt].ypos;
											setGridPosition(ActiveItems[mapPt - 1].label, ActiveItems[mapPt - 1].xpos, ActiveItems[mapPt - 1].ypos);
										}
										else{		
											uint8_t ItemDropped = Random()%10;
											if(ItemDropped < 7){
												for(itemPt = 12; itemPt < 20; itemPt++){
													if(ActiveItems[itemPt].active == 0) {
														ActiveItems[itemPt].active = 1;
														ActiveItems[itemPt].display = 1;
														uint8_t ItemType = Random()%10;
														if(ItemType < 7) ActiveItems[itemPt].type = 1;
														else if (ItemType < 9) ActiveItems[itemPt].type = 4;
														else ActiveItems[itemPt].type = 5;
														ActiveItems[itemPt].xpos = Characters[characterPt].xpos;
														ActiveItems[itemPt].ypos = Characters[characterPt].ypos;
														setGridPosition(ActiveItems[itemPt].label, ActiveItems[itemPt].xpos, ActiveItems[itemPt].ypos);
														break;
													}
												}
											}
											updateItems = itemPickedUp();
										}
									}
								}				
							}
						}
					bombCountingDown = 0;
					cloudCountdown = 4;
					cloudCountingDown = 1;
				}
}
void Character_Item_World_Init(void){
	drawMap(mapPt);
	nextMap = 2;
	Characters[characterPt].direction = South;
	Characters[characterPt].xpos = 3;
	Characters[characterPt].ypos = 5;
	Characters[characterPt].drawX = gridPositionX (3);
	Characters[characterPt].drawY = gridPositionY (5);
	spriteInit(Characters[characterPt].sprite, Characters[characterPt].xpos, Characters[characterPt].ypos,
	Characters[characterPt].direction , Characters[characterPt].label);
	for(itemPt = 0; itemPt < 20; itemPt ++){			
		ActiveItems[itemPt].active = 0;
		ActiveItems[itemPt].display = 0;
		ActiveItems[itemPt].type = 0;
		ActiveItems[itemPt].label = itemPt + 11;
		ActiveItems[itemPt].xpos = 0;
		ActiveItems[itemPt].ypos = 0;
	}
}
void Play_Game (void){
		if(!selectScreen){
			// Cleans screen of black selection box
			if(selectChange){
				selectChange = 0;
				CleanRect(gridPositionX(2), gridPositionY(7) + 1,
				gridPositionX(5) - gridPositionX(2), gridPositionY(7) - gridPositionY(1) + 1);
			}
			// Opens the boss map when all triForcePieces are collected
			if(openBoss){
				openBoss = 0;
				WorldMap[4].NextMap[1] = 13;
				WorldMap[7].NextMap[0] = 13;
			}
			// Changes to next map using FSM
			if(updateMap && (mapPt != 16 || !(bossAlive))){ update_Map(); }
			
			// Draw special maps
			if(mapPt == 15){
				drawDoorItems(swordActive, fireAnimationCounter);
				if(!(swordActive))
					setGridPosition(21, 2, 5); // Puts sword on grid
			}
			else if(mapPt == 17)	{completeScreen = 1;}
			
			if(updateItems){ update_Items(); }
			
			// updates number of sprites moving for timer
			numSpritesMoving = 0;
			for(characterPt = 0; characterPt < numCharacters; characterPt++){
				if(Characters[characterPt].isMoving) numSpritesMoving++;
			}
			
			// Puts collectable items on grid and draws them on map
			if(mapPt != 17){
				for(itemPt = 0; itemPt < 20; itemPt++){
					if(ActiveItems[itemPt].active && ActiveItems[itemPt].display){
						setGridPosition(ActiveItems[itemPt].label, ActiveItems[itemPt].xpos, ActiveItems[itemPt].ypos);
						drawItem(ActiveItems[itemPt].type, gridPositionX(ActiveItems[itemPt].xpos), gridPositionY(ActiveItems[itemPt].ypos));
					}
				}
				// if bomb used then display
				if(UseItem[0].display){ 
					drawItem(UseItem[0].type, gridPositionX(UseItem[0].xpos), gridPositionY(UseItem[0].ypos));
				}
			}
			
			// countsdown the display for the bomb cloud
			
			
			if(useItem && !(bombCountdown) && !(PotionCountDown) && !(cloudCountdown)
								 && !(cloudCountingDown) && !(bombCountingDown)){ use_Item(); }
			else{useItem = 0;}
			
			bomb_Physics();
			
			

			// Attack animation
			if(attackDown){ update_Attack(); }
			
			// Updates sprite position by moving it MOVE TO SYSTICK
			/*for(characterPt = numCharacters - 1; characterPt >= 0; characterPt--){
				if(!(characterPt == 0 && attackDown)){
					if(Characters[characterPt].isMoving && Characters[characterPt].state){
						setSpriteLocation(Characters[characterPt].sprite, &Characters[characterPt].xpos, &Characters[characterPt].ypos,
						&Characters[characterPt].drawX, &Characters[characterPt].drawY, Characters[characterPt].direction ,
						Characters[characterPt].label, &Characters[characterPt].spriteAnimationCounter, &Characters[characterPt].isMoving,
						numSpritesMoving);					
					}
				}
			}*/
			
			// Checks to see if Link was attacked by an enemy
			for(characterPt = 1; characterPt < numCharacters; characterPt++){
				if((Characters[characterPt].state) && (invicCounter == 0)){
					uint8_t linkAttacked = checkAttacked(Characters[0].direction, Characters[0].drawX, Characters[0].drawY,
					Characters[characterPt].drawX, Characters[characterPt].drawY, Characters[characterPt].sprite);
					if(linkAttacked) {Play_FX(HURT); updateScore = 1; score -= 20; Characters[0].life --; updateHeart = 1; invicCounter = 12;  break;}
				}
			}
			// Allows sprites to move in direction and picks up items
		//	if(updateSprites){ update_Sprites(); }
			// Changes the displayed hearts on the screen
			if(updateHeart){ update_Hearts(); }
			// Changes the scoor displayed
			if(updateScore){ update_Score(); }
			// End of playable game
		}
		else{ // Displayes selection screen
			if(updateSelect){
				updateSelect = 0;
				ST7735_FillRect(gridPositionX(2), gridPositionY(1) + 1,
				gridPositionX(5) - gridPositionX(2), gridPositionY(7) - gridPositionY(1) + 1, 0x0000);
				drawSentence (("SELECT ITEM"), 11, 50, centerY(11));
				drawSelection(triForcePieces, UseItem[0].amount, UseItem[1].amount);
			}
			if(activeItem == 0){
					ST7735_FillRect(83, 101, 1, 15, 0xFFFF);
					ST7735_FillRect(83, 78, 1, 15, 0x0000);
			}
			else{
					ST7735_FillRect(83, 78, 1, 15, 0xFFFF);
					ST7735_FillRect(83, 101, 1, 15, 0x0000);
			}
		}
}

/*************** SysTick_Handler *****************
	Calls for screen to be updated when loop replays
	Don't think we want screen to update here so remove extra code
	inputs: none
	outputs: none
*/
void SysTick_Handler(void){
	uint8_t data1 = GPIO_PORTE_DATA_R & 0xFF;
	uint8_t data2 = GPIO_PORTF_DATA_R & 0xFF;
	uint8_t data3 = GPIO_PORTB_DATA_R & 0xC0;  
	
	if(!((data3 & 0x40) >> 6)){ // PB6
		selectDown = 0; selectUp = 1;
	}
	if(!selectScreen && (mapPt != 17)){ // Stops movement when selection screen active or last map
		if(((data3 & 0x40) >> 6) && (selectUp)) { // PB6
			selectDown = 1; selectUp = 0; updateSelect = 1; selectScreen = 1;
		}
		if(!((data1 & 0x20) >> 5) && Characters[0].attackAnimationCounter >= 9 && swordActive){ //PE5
			attackUp = 1;
			attackDown = 0;
		}
		if(((data1 & 0x20) >> 5) && !(attackDown) && attackUp && swordActive){ // PE5; allows attacking if sword active
			Characters[0].attackAnimationCounter = 0;
			attackDown = 1;
			attackUp = 0;
		}
		else if((data3 & 0x80) >> 7) //PB7
			useItem = 1;
		else if(!((data1 & 0x02) >> 1)) // PE1
			Characters[0].moveDirection = 0;
		else if((data1 & 0x04) >> 2) // PE2
			Characters[0].moveDirection = 3;
		else if((data1 & 0x08) >> 3) // PE3
			Characters[0].moveDirection = 2;
		else if(data1 & 0x01) // PE0
			Characters[0].moveDirection = 1;
		else
			Characters[0].moveDirection = 4;
		
		uint8_t tempPt;
		for(tempPt = 1; tempPt < numCharacters; tempPt++){
			if(!(Characters[tempPt].isMoving && Characters[characterPt].state) && Characters[tempPt].sprite != 4)
				Characters[tempPt].moveDirection = Random()%10;
		}
		// updates the countdowns
		
		if(invicCounter > 0) invicCounter --;
		if(bombCountdown > 0) bombCountdown --; 
		if(PotionCountDown > 0) PotionCountDown --;
		if(cloudCountdown > 0) cloudCountdown --;
		
		if(mapPt == 15) fireAnimationCounter = (fireAnimationCounter + 1)&0x0F;
		

		update_Sprites();
		
		for(characterPt = numCharacters - 1; characterPt >= 0; characterPt--){
				if(!(characterPt == 0 && attackDown)){
					if(Characters[characterPt].isMoving && Characters[characterPt].state){
						Characters[characterPt].speedCounter ++;
						if(Characters[characterPt].speedCounter % Characters[characterPt].speedDivider == 0){
							setSpriteLocation(Characters[characterPt].sprite, &Characters[characterPt].xpos, &Characters[characterPt].ypos,
							&Characters[characterPt].drawX, &Characters[characterPt].drawY, Characters[characterPt].direction ,
							Characters[characterPt].label, &Characters[characterPt].spriteAnimationCounter, &Characters[characterPt].isMoving,
							numSpritesMoving);					
						}
					}
				}
			}
		
	}
	else{ // Selection screen active
		if(useItem) useItem = 0;
		uint16_t ADCMail = ADC_In();
		if(ADCMail < 4095/2) activeItem = 0; // ADC uses PE4
		else activeItem = 1;
		if(!((data3 & 0x40) >> 6) && (selectDown)){ // PB6
			selectUp = 1; selectDown = 0; selectScreen = 1;
		}
		else if((selectUp) && ((data3 & 0x40) >> 6)) { // PB6
			selectScreen = 0; selectDown = 1; selectUp = 0; selectChange = 1;
		}
	}
}

int main(){
	Port_Init();										// initialize port E and F and turns on clock for A, B, E, F
  TExaS_Init(); 									// set system clock to 80 MHz
  Output_Init();									// initializes ST7735 for output
	ADC_Init();											// initializes ADC; uses port 
	DAC_Init();											// initializes DAC
	I_O_Init();
	//Sound_Init();									// initialize sound driver
  //playMusic(1);									// play start sound
	ST7735_SetCursor(0, 0);
	Character_Item_World_Init();
	SysTick_Init(80000000/30);			// initializes the systick and sets period; 80Mhz/period so 30 hz for this one
	Random_Init(NVIC_ST_CURRENT_R);	// initializes random number generator
	EnableInterrupts();
  while(1){
		if(!(gameFinished)){
		if(completeScreen){
			if(moveCounter <= 4){
				if(completeTaskOnce){
					ST7735_FillRect(0, 0, 8, 160, 0x0000);
					drawFinalRoom(); completeTaskOnce = 0; Delay100ms(20);
				}
				if(Characters[0].isMoving && Characters[0].state){
					setSpriteLocation(Characters[0].sprite, &Characters[0].xpos, &Characters[0].ypos,
					&Characters[0].drawX, &Characters[0].drawY, Characters[0].direction ,
					Characters[0].label, &Characters[0].spriteAnimationCounter, &Characters[0].isMoving,
					numSpritesMoving);					
				}
				Characters[0].moveDirection = North;
				update_Sprites();
			}
			else{
				Characters[0].direction = South;
				drawSprite (Characters[0].sprite, Characters[0].label, Characters[0].drawX,
				Characters[0].drawY, Characters[0].direction, Characters[0].spriteAnimationCounter);
					
				Delay100ms(40);
				drawSentence(("QUEST COMPLETE"), 14, 9, centerY(14));
				Delay100ms(15);
				Play_FX(GET_HEART);
				
				ST7735_FillRect(gridPositionX(2), gridPositionY(1) + 1,
				gridPositionX(5) - gridPositionX(2), gridPositionY(7) - gridPositionY(1) + 1, 0x0000);
				drawSentence (("SCORE"), 5, 50, centerY(5));
				
				uint8_t digits;
				if(score < 0) {score = 0; digits = 1;}
				else if(score < 10) digits = 1;
				else if(score < 100) digits = 2;
				else if(score < 1000) digits = 3;
				else if(score < 10000) digits = 4;
				drawNumber(score, digits, 70, centerY(digits));
				gameFinished = 1;
			}
		}
		else if(!(completeScreen) && (Characters[0].life > 0 || invincibility ))
			Play_Game();
		else if(deathScreen){
				Play_FX(DEATH);
				ST7735_FillScreen(0x01DF); 
				ST7735_FillRect(55, 160 - centerY(9) - 5, 15, 65, 0x0000);
				drawSentence(("GAME OVER"), 9, 60, centerY(9));
				deathScreen = 0;
				NVIC_ST_CTRL_R = 0;
		}
	}
	}
}

// You can use this timer only if you learn how it works
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
