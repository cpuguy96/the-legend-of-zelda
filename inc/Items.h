#ifndef __Items_h
#define __Items_h

void drawItem (uint8_t Index, int16_t drawX, int16_t drawY);
void cleanItem (uint8_t Index, uint8_t xPosition, uint8_t yPosition);
void drawSelection (uint8_t TriforcePieces, uint8_t BombAmount, uint8_t PotionAmount);
void drawCloud (uint8_t xPosition, uint8_t yPosition, uint8_t cloudCountdown);
void drawDoorItems (uint8_t swordActive, uint8_t fireAnimationCounter);
void drawFinalRoom (void);

#endif
