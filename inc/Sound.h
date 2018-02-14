// Sound.h
// Runs on TM4C123
// November 17, 2014

#ifndef __Sound_h
#define __Sound_h

enum SONG{INTRO, OVERWORLD, DUNGEON, NUM_SONGS};
enum SOUND_FX{KILL, SWORD_SLASH, HIT, HURT, GET_HEART, ARROW, BOMB_BLOW, BOSS_SCREAM, DEATH, SECRET, NUM_SOUND_FX};

void Sound_Init(void);
void Play_Song(uint8_t song);
void Play_FX(int16_t soundFX);

#endif
