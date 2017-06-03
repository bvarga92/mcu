#include "conway.h"

/* bit 0: van-e itt elo sejt (1) vagy nincs (0), bit1: kovetkezo allapot */
static uint8_t table[NUM_CELLS_X][NUM_CELLS_Y];

static void draw(void){
	uint16_t x, y;
	for(y=0;y<NUM_CELLS_Y;y++)
		for(x=0;x<NUM_CELLS_X;x++)
			gDrawFilledRectangle(x*CELL_SIZE_X,y*CELL_SIZE_Y,CELL_SIZE_X,CELL_SIZE_Y,(table[x][y]&0x01)?COLOR_CELL:COLOR_BG);
}

static void newGame(void){
	uint16_t x, y;
	for(y=0;y<NUM_CELLS_Y;y++)
		for(x=0;x<NUM_CELLS_X;x++)
			table[x][y]=(randomGet()%2)?0x03:0x00;
	draw();
}

static uint8_t countNeighbours(uint16_t x, uint16_t y){
	uint16_t left=(x>0)?(x-1):(NUM_CELLS_X-1);
	uint16_t right=(x<(NUM_CELLS_X-1))?(x+1):0;
	uint16_t below=(y>0)?(y-1):(NUM_CELLS_Y-1);
	uint16_t above=(y<(NUM_CELLS_Y-1))?(y+1):0;
	uint8_t cntr=0;
	if(table[left][below]&0x01) cntr++;
	if(table[left][y]&0x01) cntr++;
	if(table[left][above]&0x01) cntr++;
	if(table[x][below]&0x01) cntr++;
	if(table[x][above]&0x01) cntr++;
	if(table[right][below]&0x01) cntr++;
	if(table[right][y]&0x01) cntr++;
	if(table[right][above]&0x01) cntr++;
	return cntr;
}

static void update(void){
	uint16_t x, y;
	uint8_t neighbours;
	for(y=0;y<NUM_CELLS_Y;y++){
		for(x=0;x<NUM_CELLS_X;x++){
			neighbours=countNeighbours(x,y);
			if((neighbours<2)||(neighbours>3))
				table[x][y]&=0xFD; //elszigetelodes, tulnepesedes --> elpusztul
			else if(neighbours==3)
				table[x][y]|=0x02; //uj sejt szuletik
		}
	}
	for(y=0;y<NUM_CELLS_Y;y++)
		for(x=0;x<NUM_CELLS_X;x++)
			table[x][y]=(table[x][y]&0x02)?0x03:0x00;
}

void conway(float a, float b, float c, float d){
	newGame();
	while(!buttonRead()){
		touchRead();
		if(touchData.status==TOUCH_PRESSED) newGame();
		update();
		draw();
		HAL_Delay(20);
	}
}
