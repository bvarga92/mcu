#ifndef _CONWAY_H_
#define _CONWAY_H_

	#include "random.h"
	#include "graphics.h"
	#include "system.h"
	#include "touch.h"

	#define  NUM_CELLS_X   120
	#define  NUM_CELLS_Y   68
	#define  CELL_SIZE_X   480/NUM_CELLS_X
	#define  CELL_SIZE_Y   272/NUM_CELLS_Y
	#define  COLOR_CELL    COLOR_BLACK
	#define  COLOR_BG      COLOR_WHITE

	void conway(float a, float b, float c, float d);

#endif
