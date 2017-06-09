#ifndef _FRACTALS_H_
#define _FRACTALS_H_

	#include <math.h>
	#include "random.h"
	#include "graphics.h"

	#define MAX_ITERATIONS 50

	void initPalette(uint8_t colormap);
	void julia(float xmin, float xmax, float ymin, float ymax);
	void mandelbrot(float xmin, float xmax, float ymin, float ymax);
	void sierpinski(float xmin, float xmax, float ymin, float ymax);
	void koch(float a, float b, float c, float d);

#endif
