#include "fractals.h"

static uint16_t palette[MAX_ITERATIONS];

void initPalette(uint8_t colormap){
	uint16_t i;
	switch(colormap){
		case 0: //teljes paletta pirostol
			for(i=0;i<MAX_ITERATIONS;i++)
				palette[i]=hsv2rgb565((uint16_t)floor(i*360/(float)MAX_ITERATIONS),100,100);
			break;
		case 1: //teljes paletta kektol
			for(i=0;i<MAX_ITERATIONS;i++)
				palette[i]=hsv2rgb565((((uint16_t)floor(i*360/(float)MAX_ITERATIONS))+240)%360,100,100);
			break;
		case 2: //kek
			for(i=0;i<MAX_ITERATIONS;i++)
				palette[i]=hsv2rgb565((uint16_t)floor(i*120/(float)MAX_ITERATIONS+180),100,100);
			break;
		case 3: //zold
			for(i=0;i<MAX_ITERATIONS;i++)
				palette[i]=hsv2rgb565((uint16_t)floor(i*120/(float)MAX_ITERATIONS+60),100,100);
			break;
		default: //feher
			for(i=0;i<MAX_ITERATIONS;i++)
				palette[MAX_ITERATIONS-i-1]=hsv2rgb565(0,0,(uint8_t)floor(i*100/(float)MAX_ITERATIONS));
	}
}

void julia(float xmin, float xmax, float ymin, float ymax){
	uint16_t i, j, iteration, color;
	float x0, y0, x, y, xtemp;
	for(i=0;i<480;i++){
		x0=i/479.0*(xmax-xmin)+xmin;
		for(j=0;j<272;j++){
			y0=j/271.0*(ymin-ymax)+ymax;
			x=x0;
			y=y0;
			iteration=0;
			while((x*x+y*y<4)&&(iteration<MAX_ITERATIONS)){
				xtemp=x*x-y*y-0.7;
				y=2*x*y+0.27015;
				x=xtemp;
				iteration++;
			}
			color=(iteration>MAX_ITERATIONS-1)?COLOR_BLACK:palette[iteration];
			gDrawPixel(i,j,color);
		}
	}
}

void mandelbrot(float xmin, float xmax, float ymin, float ymax){
	uint16_t i, j, iteration, color;
	float x0, y0, x, y, xtemp;
	for(i=0;i<480;i++){
		x0=i/479.0*(xmax-xmin)+xmin;
		for(j=0;j<272;j++){
			y0=j/271.0*(ymin-ymax)+ymax;
			x=0.0;
			y=0.0;
			iteration=0;
			while((x*x+y*y<4)&&(iteration<MAX_ITERATIONS)){
				xtemp=x*x-y*y+x0;
				y=2*x*y+y0;
				x=xtemp;
				iteration++;
			}
			color=(iteration>MAX_ITERATIONS-1)?COLOR_BLACK:palette[iteration];
			gDrawPixel(i,j,color);
		}
	}
}

void sierpinski(float xmin, float xmax, float ymin, float ymax){
	uint32_t i;
	float w, h, ax, ay, bx, by, cx, cy, px, py;
	w=960.0/(xmax-xmin);
	h=544.0/(ymax-ymin);
	ax=-(1+xmin)*0.5*w; ay=(1+ymax)*0.5*h;
	bx=ax+w; by=ay;
	cx=(ax+bx)*0.5; cy=ay-h;
	px=ax; py=ay;
	gFill(COLOR_WHITE);
	for(i=0;i<1600000;i++){
		if((px>=0)&&(py>=0)&&(px<480)&&(py<272)) gDrawPixel((uint16_t)px,(uint16_t)py,COLOR_BLACK);
		switch(randomGet()%3){
			case 0:
				px=(px+ax)/2.0;
				py=(py+ay)/2.0;
				break;
			case 1:
				px=(px+bx)/2.0;
				py=(py+by)/2.0;
				break;
			case 2:
				px=(px+cx)/2.0;
				py=(py+cy)/2.0;
				break;
		}
	}
}

static void kochsub(uint8_t level, int16_t x1, int16_t y1, int16_t x5, int16_t y5){
	int16_t dx, dy, x2, y2, x3, y3, x4, y4;
	if(level==0){
		gDrawLine(x1,y1,x5,y5,COLOR_BLACK);
	}
	else{
		dx=x5-x1;
		dy=y5-y1;
		x2=x1+dx/3;
		y2=y1+dy/3;
		x3=(int16_t)(0.5*(x1+x5)+sqrt(3)*(y1-y5)/6);
		y3=(int16_t)(0.5*(y1+y5)+sqrt(3)*(x5-x1)/6);
		x4=x1+2*dx/3;
		y4=y1+2*dy/3;
		kochsub(level-1,x1,y1,x2,y2);
		kochsub(level-1,x2,y2,x3,y3);
		kochsub(level-1,x3,y3,x4,y4);
		kochsub(level-1,x4,y4,x5,y5);
	}
}

void koch(float a, float b, float c, float d){ //dummy parameterek
	uint8_t depth;
	for(depth=0;depth<6;depth++){
		gFill(COLOR_WHITE);
		kochsub(depth,140,207,342,207);
		kochsub(depth,342,207,241,5);
		kochsub(depth,241,5,140,207);
		HAL_Delay(1000);
	}
}
