#ifndef __Sentence_h
#define __Sentence_h

void drawSentence (char ASCIIcharacter[20], uint8_t length, int16_t drawX, int16_t drawY);
void drawNumber (uint16_t number, uint8_t digits, int16_t drawX, int16_t drawY);
void drawCharacter (uint8_t Index, int16_t drawX, int16_t drawY);
int16_t centerY(uint8_t length);
int16_t floatRight(uint8_t length);

#endif
