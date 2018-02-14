// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders 
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>
#include "../inc/Sound.h"
#include "../inc/DAC.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Sound_Constants.h"
// cur_sound = -1 when there is no sound effect (will be playing music)
// others = -1 means not activated

void (*PeriodicTask1)(void);   // user function

uint8_t CurrentSound = 0;
int8_t cur_music = 0;

static uint8_t CurrentSong;
static int16_t CurrentFX;
static uint32_t FXIndex[NUM_SOUND_FX] = {0};
static uint32_t SongIndex[NUM_SONGS] = {0};



// pass a parameter 0,1 or 2 for different music
void Play_Song(uint8_t song) {
	CurrentSong = song;
}
void Play_FX(int16_t soundFX){
	CurrentFX = soundFX;
	FXIndex[CurrentFX] = 0;
}

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init(uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	if(CurrentFX >= 0){
		if(FXIndex[CurrentFX] < FX_LENGTH[CurrentFX]){
			DAC_Out(FX[CurrentFX][FXIndex[CurrentFX]]);
			FXIndex[CurrentFX] ++;
		}
		else{
			FXIndex[CurrentFX] = 0;
			CurrentFX = -1;
		}
	}
	else{
		DAC_Out(SONGS[CurrentSong][SongIndex[CurrentSong]]);
		SongIndex[CurrentSong] = (SongIndex[CurrentSong] + 1) % SONGS_LENGTH[CurrentSong];
	}
}


void Sound_Init() {
	DAC_Init();
	Timer1_Init(80000000/11025); // make seperate interrupts for FX and music
	CurrentSong = 0;
	CurrentFX = -1;
}

/*void SoundTask(){
	switch (cur_sound) {
		case 0:
			if (index_kill < 2049){
				DAC_Out(KILL[index_kill]);
				index_kill++;
			} else cur_sound = -1;	// back to play music
			break;
		case 1:
			if (index_sword < 1671){
				DAC_Out(SWORD_SLASH[index_sword]);
				index_sword++;
			} else cur_sound = -1;	// back to play music
			break;
		case 2:
			if (index_hit < 2695){
				DAC_Out(HIT[index_hit]);
				index_hit++;
			} else cur_sound = -1;	// back to play music
			break;
		case 3:
			if (index_hurt < 754){
				DAC_Out(HURT[index_hurt]);
				index_hurt++;
			} else cur_sound = -1;	// back to play music
			break;
		case 4:
			if (index_getheart < 1140){
				DAC_Out(GET_HEART[index_getheart]);
				index_getheart++;
			} else cur_sound = -1;	// back to play music
			break;
		case 5:
			if (index_arrow < 868){
				DAC_Out(ARROW[index_arrow]);
				index_arrow++;
			} else cur_sound = -1;	// back to play music
			break;
			
		case 6:
			if (index_bombBlow < 4221){
				DAC_Out(BOMB_BLOW[index_bombBlow]);
				index_bombBlow++;
			} else cur_sound = -1;	// back to play music
			break;
			
		case 7:
			if (index_bossScream2 < 8367) {
				DAC_Out(BOSS_SCREAM[index_bossScream2]);
				index_bossScream2++;
			} else cur_sound = -1;
			break;
			
		case 8:
			if (index_die < 27776) {
				DAC_Out(DEATH[index_die]);
				index_die++;
			} else cur_sound = -1;
			break;
			
		case 9:
			if (index_secret < 17827) {
				DAC_Out(SECRET[index_secret]);
				index_secret++;
			} else cur_sound = -1;
			break;

		default:	// play music
			if (cur_music == 0){
				DAC_Out(INTRO[index_intro]);
				index_intro = (index_intro + 1) % 30359 ;
			}
			if (cur_music == 1){
				DAC_Out(OVERWORLD[index_overworld]);
				index_overworld = (index_overworld + 1) % 35632 ;
			}
			if (cur_music == 2){
				DAC_Out(DUNGEON[index_dungeon]);
				index_dungeon = (index_dungeon + 1) % 30370 ;
			}
		
	}
}*/
