#include "system.h"
#include "touch.h"
#include "lcd.h"
#include "graphics.h"
#include "courier20b_ext.h"
#include "fractals.h"
#include "conway.h"

float xmin, width, ymin, height;

void (*selected)(float,float,float,float);

void gui(void){
	gFill(COLOR_WHITE);
	gPutString(5,8,"Julia-halmaz",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	gPutString(5,48,"Mandelbrot-halmaz",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	gPutString(5,88,"Sierpi\xF1ski-h\xE1romsz\xF6g",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	gPutString(5,128,"Koch-h\xF3pehely",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	gPutString(5,168,"Conway-sejtautomata",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	while(1){
		touchRead();
		if(touchData.status==TOUCH_RELEASED) continue;
		if((touchData.p[0].x>=5)&&(touchData.p[0].x<=173)&&(touchData.p[0].y>=8)&&(touchData.p[0].y<=28)){
			xmin=-1.5; width=3.0; ymin=-1.0; height=2.0;
			selected=&julia;
			break;
		}
		if((touchData.p[0].x>=5)&&(touchData.p[0].x<=243)&&(touchData.p[0].y>=48)&&(touchData.p[0].y<=68)){
			xmin=-2.5; width=3.5; ymin=-1.0; height=2.0;
			selected=&mandelbrot;
			break;
		}
		if((touchData.p[0].x>=5)&&(touchData.p[0].x<=285)&&(touchData.p[0].y>=88)&&(touchData.p[0].y<=108)){
			xmin=-1.0; width=2.0; ymin=-1.0; height=2.0;
			selected=&sierpinski;
			break;
		}
		if((touchData.p[0].x>=5)&&(touchData.p[0].x<=187)&&(touchData.p[0].y>=128)&&(touchData.p[0].y<=148)){
			selected=&koch;
			break;
		}
		if((touchData.p[0].x>=5)&&(touchData.p[0].x<=266)&&(touchData.p[0].y>=168)&&(touchData.p[0].y<=188)){
			selected=&conway;
			break;
		}
	}
}

int main(void){
	configMPU();
	enableCache();
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	touchInit();
	lcdInit();
	lcdInitLayers();
	lcdSetLayer2();
	initPalette(1);
	gui();
	selected(xmin,xmin+width,ymin,ymin+height);
	while(1){
		touchRead();
		if(touchData.status==TOUCH_PRESSED){
			/* az erintes helyet kozepre tesszuk */
			xmin=(float)(touchData.p[0].x)/479.0*width+xmin-width*0.5;
			ymin=(271.0-(float)(touchData.p[0].y))/271.0*height+ymin-height*0.5;
			/* belezoomolunk */
			width*=0.5; height*=0.5;
			xmin+=width*0.5; ymin+=height*0.5;
			selected(xmin,xmin+width,ymin,ymin+height);
		}
		else if(buttonRead()){
			gui();
			selected(xmin,xmin+width,ymin,ymin+height);
		}
	}
	return 1;
}
