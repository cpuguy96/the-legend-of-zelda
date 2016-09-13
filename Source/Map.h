void drawMap (uint8_t Index);
void drawTile (int8_t xIndex, int8_t yIndex);
void reDrawTile (uint8_t xIndex, uint8_t yIndex, int16_t drawX, int16_t drawY, uint8_t width, uint8_t height, uint8_t direction);
const unsigned short * getMapTile(uint8_t xIndex, uint8_t yIndex);
