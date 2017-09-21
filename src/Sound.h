// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// November 17, 2014


void Sound_Init(void);
void playMusic(uint8_t music);

void playHit(void);
void playHurt(void);
void playKill(void);
void playSword(void);
void playGetHeart(void);
void playArrow(void);
void playBombBlow(void);
void playBossScream2(void);
void playDie(void);
void playSecret(void);


