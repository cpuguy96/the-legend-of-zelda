
#ifndef __Map_p_h
#define __Map_p_h
/* 
BLBush 		0
BMBush 		1
BRBush 		2
Door			3
Floor			4
TLBush		5
TMBush		6
TRBush		7
dungTile	8
Bush			9
FaBush 		10
BFaBush		11
BBBush		12
BBLBush		13
BBRBush		14
BTBush		15
BTLBush		16
BTRBush		17	
BBush			18
TOpen			19
BROpen		20
TROpen		21
TLOpen		22
BLOpen		23
GFLoor		24
BBushG		25
TLBushG		26
TBushG		27
TRBushG		28
TMOpen		29
DFloor		30
TrapG			31
*/

const uint8_t OVERWORLD_MAP0 [80] = {
	1, 1, 1, 4, 6, 1, 1, 1,
	1, 1, 1, 4, 4, 4, 6, 1,
	1, 1, 1, 4, 4, 4, 6, 1,
	1, 1, 0, 4, 4, 4, 6, 1,
	4, 4, 4, 4, 4, 4, 6, 1,
	4, 4, 4, 4, 4, 4, 6, 1,
	2, 4, 4, 4, 4, 4, 6, 1,
	3, 4, 4, 4, 4, 4, 6, 1,
	1, 2, 4, 4, 4, 4, 6, 1,
	1, 1, 2, 4, 7, 1, 1, 1,
};
const uint8_t OVERWORLD_MAP1 [80] = {
	1, 1, 0, 4, 4, 5, 1, 1,
	1, 0, 4, 4, 4, 4, 6, 1,
	1, 4, 4,10,10, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 4, 4,
	1, 4,10, 4,10, 4, 4, 4,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4,10, 4,10, 4, 6, 1,
	1, 2, 4, 4, 4, 4, 6, 1,
	1, 1, 2, 4, 4, 7, 1, 1,
};
const uint8_t OVERWORLD_MAP2 [80] = {
	9, 9, 4, 4, 4, 4, 9, 9,
	4, 4, 4, 4, 9, 4, 9, 9,
	9, 9, 4, 9, 4, 4, 9, 9,
	4, 4, 4, 4, 9, 4, 9, 9,
	9, 9, 4, 9, 4, 4, 9, 9,
	4, 4, 4, 4, 9, 4, 9, 9,
	9, 9, 4, 9, 4, 4, 9, 9,
	4, 4, 4, 4, 4, 4, 9, 9,
	1, 2, 4, 4, 4, 7, 1, 1,
	1, 1, 2, 4, 7, 1, 1, 1,
};
const uint8_t OVERWORLD_MAP3 [80] = {
	1, 1, 0, 4, 5, 1, 1, 1,
	1, 0, 4, 4, 4, 5, 1, 1,
	4, 4, 4,10, 4, 4, 6, 1,
	4, 4, 4, 4, 4, 4, 6, 1,
	2, 4, 4, 4, 4, 4, 6, 1,
	1, 4,10, 4,10, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4,10, 4,10, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
};

const uint8_t OVERWORLD_MAP4[80] = {
	9,9,9,4,4,9,9,9,
	9,4,4,4,4,4,4,4,
	9,9,4,9,9,4,9,9,
	9,4,4,4,9,4,4,4,
	4,4,4,9,9,4,9,9,
	4,4,4,9,9,4,4,4,
	9,9,4,9,4,4,9,9,
	9,9,4,9,9,4,4,4,
	1,2,4,4,4,4,7,1,
	1,1,2,4,4,7,1,1
};

const uint8_t OVERWORLD_MAP5[80] = {
	12,12,12,12,12,12,12,12,
	12,12,13, 4, 4,16,12,12,
	12,12, 4, 4, 4, 4,15,12,
	12,12, 4, 9, 9, 4,15,12,
	12,12, 4, 4, 4, 4, 4, 4,
	12,12, 4, 4, 4, 4, 4, 4,
	12,12, 4, 9, 9, 4,15,12,
	12,12, 4, 4, 4, 4,15,12,
	12,12,14, 4, 4,17,12,12,
	12,12,12,12,12,12,12,12
};

const uint8_t OVERWORLD_MAP6[80] = {
	1, 1, 0, 4, 4, 5, 1, 1,
	1, 0, 4, 4, 4, 4, 5, 1,
	1, 4, 4, 4, 4, 4, 4, 4,
	1, 4,11,11,11, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 7, 1,
	4, 4, 4, 4, 4, 4, 6, 1,
	1, 4,11, 4,11, 4, 6, 1,
	1, 4,11, 4,11, 4, 6, 1,
	1, 4,11, 4,11, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1
};

const uint8_t OVERWORLD_MAP7[80] = {
12,12,12,12,12,12,12,12,
12,12,13, 4, 4,16,12,12,
12,13, 4, 4, 4, 4,15,12,
 4, 4, 4, 4, 4, 4,15,12,
 4, 4, 4, 4, 4, 4,15,12,
12,12,12,14, 4 ,4,15,12,
12,12,12,12, 4, 4,15,12,
12,12,12,13, 4, 4,15,12,
12,12,13, 4, 4, 4,15,12,
12,13, 4, 4, 4, 4,16,12
};

const uint8_t OVERWORLD_MAP8[80] = {
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 2, 4,10,10, 4, 6, 1,
	1, 0, 4, 4,10, 4, 6, 1,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4,10, 4, 4, 4, 4,
	2, 4, 4, 4,10, 4, 6, 1,
	1, 4, 4,10, 4, 4, 6, 1,
	1, 2, 4, 4, 4, 7, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

const uint8_t OVERWORLD_MAP9[80] = {
	12,12,12,12,12,12,12,12,
	18,18, 4, 4,16,12,12,12,
	18,18, 4, 4, 4,16,12,12,
	18,18, 4, 4, 4, 4, 4, 4,
	18,18,18, 4, 4, 4, 4, 4,
	18,18,18, 4, 4, 4,17,12,
	18,18, 4, 4, 4,17,12,12,
	18,18, 4, 4, 4,15,12,12,
	18,18, 4, 4, 4,15,12,12,
	12,12,14, 4, 4,12,12,12
};

const uint8_t OVERWORLD_MAP10[80] = {
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 4, 4,11,11, 4, 6, 1,
	1, 4, 4, 4,11, 4, 5, 1,
	1, 4, 4, 4, 4, 4, 4, 4,
	1, 4, 4, 4,11, 4, 4, 4,
	1, 4, 4,11,11, 4, 7, 1,
	1, 4, 4, 4, 4, 4, 6, 1,
	1, 2, 4, 4, 4, 7, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};
const uint8_t OVERWORLD_MAP11[80] = {
	 1, 1, 1, 1, 1, 1, 1, 1,
	 1, 1, 0, 4, 4, 5, 1, 1,
	 1, 1, 4, 4, 4, 4, 6, 1,
	 1, 1, 4,18,18, 4, 6, 1,
	 4, 4, 4, 4, 4, 4, 6, 1,
	 4, 4, 4, 4, 4, 4, 6, 1,
	 1, 1, 4,18,18, 4, 6, 1,
	 1, 1, 4, 4, 4, 4, 6, 1,
	 1, 1, 2, 4, 4, 7, 1, 1,
	 1, 1, 1, 1, 1, 1, 1, 1
};
const uint8_t OVERWORLD_MAP13[80] = {
	12,12,12,12,12,12,12,12,
	18,18, 4, 4,16,12,12,12,
	18,18, 4, 4, 4,16,12,12,
	18,18, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4,17,12,
	18,18, 4, 4, 4,17,12,12,
	18,18, 4, 4, 4,15,12,12,
	18,18, 4, 4, 4,15,12,12,
	12,12,14, 4, 4,12,12,12
};
const uint8_t OVERWORLD_MAP14[80] = {
	25,25,25,25,25,25,25,25,
	25,24,24,24,24,26,25,25,
	25,24,24,24,24,24,27,25,
	25,24,21,20,24,24,27,25,
	25,24,19, 3,24,24,24,24,
	25,24,19, 3,24,24,24,24,
	25,24,22,23,24,24,27,25,
	25,24,24,24,24,24,27,25,
	25,24,24,24,24,28,25,25,
	25,25,25,25,25,25,25,25
};
const uint8_t DOOR_MAP1 [80] = {
	25,25,25,25,25,25,25,25,
	25,30,30,30,30,30,25,25,
	25,30,30,30,30,30,25,25,
	25,30,30,30,30,30,25,25,
	25,30,30,30,30,30,30,30,
	25,30,30,30,30,30,30,30,
	25,30,30,30,30,30,25,25,
	25,30,30,30,30,30,25,25,
	25,30,30,30,30,30,25,25,
	25,25,25,25,25,25,25,25,
};
const uint8_t DUNGEON_MAP1 [80] = {
	31,31,31,31,31,31,31,31,
	31, 8, 8, 8, 8, 8, 8,31,
	31, 8, 8, 8, 8, 8, 8,31,
	31, 8, 8, 8, 8, 8, 8,31,
	 8, 8, 8, 8, 8, 8, 8,31,
	 8, 8, 8, 8, 8, 8, 8,31,
	31, 8, 8, 8, 8, 8, 8,31,
	31, 8, 8, 8, 8, 8, 8,31,
	31, 8, 8, 8, 8, 8, 8,31,
	31,31,31,31,31,31,31,31,
};
const uint8_t DUNGEON_MAP2 [80] = {
	30,30,30,30,30,30,30,30,
	30,30,30,30,30,30,30,30,
	30,31,31,31,31,31,30,30,
	30,31, 8, 8, 8,31,31,31,
	30,31, 8, 8, 8, 8, 8, 8,
	30,31, 8, 8, 8, 8, 8, 8,
	30,31, 8, 8, 8,31,31,31,
	30,31,31,31,31,31,30,30,
	30,30,30,30,30,30,30,30,
	30,30,30,30,30,30,30,30,
};
const uint8_t OVERWORLD_MAP12[80] = {
	12,12,12,12,12, 1, 1, 1,
	12,12,13, 4, 4,16, 1, 1,
	12,12, 4, 4, 4, 4,15, 1,
	12,12, 4,18, 9, 4,15, 1,
	12,12, 4, 4, 4, 4, 4, 4,
	12,12, 4, 4, 4, 4, 4, 4,
	12,12, 4,18, 9, 4,15, 1,
	12,12, 4, 4, 4, 4,15, 1,
	12,12,14, 4, 4,17, 1, 1,
	12,12,12,12,12, 1, 1, 1
};

const uint8_t* MAPS[] = {
	OVERWORLD_MAP0, OVERWORLD_MAP1, OVERWORLD_MAP2, OVERWORLD_MAP3, OVERWORLD_MAP4, OVERWORLD_MAP5, OVERWORLD_MAP6, OVERWORLD_MAP7, OVERWORLD_MAP8, OVERWORLD_MAP9, OVERWORLD_MAP10,
	OVERWORLD_MAP11, OVERWORLD_MAP12, OVERWORLD_MAP13, OVERWORLD_MAP14, DOOR_MAP1, DUNGEON_MAP1, DUNGEON_MAP2,
};

#endif
