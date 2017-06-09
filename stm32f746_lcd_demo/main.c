/***********************************************************/
/*                                                         */
/* LCD demo a Discovery kartyahoz.      Varga Balazs 2016  */
/*                                                         */
/* Egy rovid animacio utan rajzolhatunk a kijelzore. A     */
/* gombbal ki- es bekapcsolhatjuk a hattervilagiatast.     */
/*                                                         */
/***********************************************************/

#include "system.h"
#include "touch.h"
#include "lcd.h"
#include "graphics.h"
#include "courier20b_ext.h"
#include "image.h"

#define MY_COLOR RGB2COLOR(38,152,222)

void clearCanvas(void){
	gFill(COLOR_WHITE);
	gDrawFilledRectangle(450,0,30,30,COLOR_RED);
	gDrawLine(454,4,475,25,COLOR_WHITE);
	gDrawLine(454,25,475,4,COLOR_WHITE);
	gPutString(5,8,"Rajzolj az ujjaddal!",courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
}

int main(void){
	uint16_t lastX=0, lastY=0, btnStatus=0, i;
	touchStatus_t lastStatus=TOUCH_RELEASED;
	/* inicializalas */
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
	/* egy kis animacio */
	gFill(COLOR_WHITE);
	for(i=0;i<272;i+=8){
		gDrawLine(0,0,479,i,COLOR_MAGENTA);
		HAL_Delay(15);
	}
	for(i=0;i<272;i+=8){
		gDrawLine(479,271,0,271-i,COLOR_MAGENTA);
		HAL_Delay(15);
	}
	for(i=0;i<272;i+=8){
		gDrawLine(479,0,0,i,COLOR_MAGENTA);
		HAL_Delay(15);
	}
	for(i=0;i<272;i+=8){
		gDrawLine(0,271,479,271-i,COLOR_MAGENTA);
		HAL_Delay(15);
	}
	HAL_Delay(500);
	for(i=4;i<136;i+=4){
		gDrawFilledCircle(240,136,i,COLOR_BLACK);
		HAL_Delay(25);
	}
	gFill(COLOR_WHITE);
	gDrawImage(&img,0,0);
	while(touchData.status==TOUCH_RELEASED) touchRead(); //erintesre varunk
	clearCanvas();
	/* most mar rajzolhatunk a kepernyore */
	while(1){
		HAL_Delay(20);
		if(buttonRead()&&(btnStatus==0)){
			btnStatus=1;
			lcdBackLightToggle();
			ledToggle();
		}
		else if((!buttonRead())&&(btnStatus==1)){
			btnStatus=0;
		}
		touchRead();
		if(touchData.cnt>1) continue; //nincs multitouch
		if(touchData.status==TOUCH_PRESSED){
			if(lastStatus==TOUCH_PRESSED){ //folytonos rajzolas, vonalat huzunk
				if(!((touchData.p[0].x>=449)&&(touchData.p[0].y<=29))) gDrawLine(lastX,lastY,touchData.p[0].x,touchData.p[0].y,MY_COLOR);
			}
			else{ //uj erintes, pixelt rajzolunk
				if((touchData.p[0].x>=449)&&(touchData.p[0].y<=29)) //megnyomtak a gombot
					clearCanvas();
				else
					gDrawPixel(touchData.p[0].x,touchData.p[0].y,MY_COLOR);
			}
			lastX=touchData.p[0].x;
			lastY=touchData.p[0].y;
		}
		lastStatus=touchData.status;
	}
	return 1;
}
