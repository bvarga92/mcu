#include "system.h"
#include "lcd.h"
#include "graphics.h"
#include "courier20b_ext.h"
#include "max30100.h"
#include "stdbool.h"

#define UINT16_TO_FLOAT1(u16) ((u16)/32768.0f-1.0f)
#define FLOAT1_TO_DISPLAY(f1) (242-(uint8_t)((f1+1.0f)*100.0f))
#define CLIP(f)               (((f)<-1.0f)?-1.0f:(((f)>1.0f)?1.0f:(f)))
#define ALPHA_DC              0.95f
#define A1                    -2.543022370324160f
#define A2                    2.238425126270596f
#define A3                    -0.676568690179719f
#define B0                    0.007010668575912f
#define B1                    0.002406364307446f
#define B2                    0.002406364307446f
#define B3                    0.007010668575912f
#define THRESHOLD             0.7f
#define ALPHA_BPM             0.01f

float bufIR[480], bpmSignal[480];

void filter(float in, float *out){
	static float m, m_next, in_dl[3], out_dl[3];
	/* egyenszint eltavolitasa exponencialis atlagolassal */
	m_next=in+ALPHA_DC*m;
	in=CLIP(-120.0f*(m_next-m)-0.4f);
	m=m_next;
	/* 10 Hz-es, harmadfoku, elliptikus IIR alulatereszto szuro */
	*out=CLIP(B0*in+B1*in_dl[0]+B2*in_dl[1]+B3*in_dl[2]-A1*out_dl[0]-A2*out_dl[1]-A3*out_dl[2]);
	in_dl[2]=in_dl[1]; in_dl[1]=in_dl[0]; in_dl[0]=in;
	out_dl[2]=out_dl[1]; out_dl[1]=out_dl[0]; out_dl[0]=*out;
}

float detectBPM(uint16_t writeIdx){
	float bpm;
	static float bpmAvg=0;
	uint16_t i, firstBeat=0, lastBeat=0, numBeats=0;
	/* derivalas, negyzetre emeles */
	for(i=0;i<479;i++){
		bpmSignal[i]=bufIR[(writeIdx+i+1)%480]-bufIR[(writeIdx+i)%480];
		if(bpmSignal[i]<0) bpmSignal[i]=0;
		bpmSignal[i]=CLIP(bpmSignal[i]*bpmSignal[i]*100.0f-1.0f);
	}
	/* csucsok keresese */
	for(i=1;i<478;i++){
		if(bpmSignal[i]<=THRESHOLD && bpmSignal[i+1]>THRESHOLD){
			numBeats++;
			if(firstBeat==0) firstBeat=lastBeat=i;
			lastBeat=i;
		}
	}
	if(numBeats==0 || lastBeat==firstBeat)
		bpm=0.0f;
	else
		bpm=numBeats*6000.0f/(lastBeat-firstBeat);
	/* exponencialis atlagolas */
	bpmAvg=ALPHA_BPM*bpm+(1-ALPHA_BPM)*bpmAvg;
	return bpmAvg;
}

void plot(uint16_t writeIdx, uint8_t bpm, bool showBpmSignal){
	static char str[128];
	uint16_t i, j;
	uint8_t curIR, prevIR, curBPM, prevBPM;
	gFill(COLOR_WHITE);
	for(i=0, j=writeIdx ; i<480 ; i++, j=(j==0)?479:(j-1)){
		curIR=FLOAT1_TO_DISPLAY(bufIR[j]);
		curBPM=FLOAT1_TO_DISPLAY(bpmSignal[479-i]);
		if(i==0){
			gDrawPixel(479-i,curIR,COLOR_BLACK);
			if(showBpmSignal) gDrawPixel(479-i,curBPM,COLOR_RED);
		}
		else{
			gDrawLine(479-i,curIR,479-i,prevIR,COLOR_BLACK);
			if(showBpmSignal) gDrawLine(479-i,curBPM,479-i,prevBPM,COLOR_RED);
		}
		prevIR=curIR;
		prevBPM=curBPM;
	}
	sprintf(str,"BPM: %d    ",bpm);
	gPutString(5,8,str,courier20b_ext,20,COLOR_BLACK,COLOR_WHITE);
	lcdSwap();
}

int main(void){
	uint16_t rawRed, rawIR, writeIdx, i;
	uint8_t showBpmSignal=0;
	float filteredIR;
	configMPU();
	enableCache();
	HAL_Init();
	configSystemClock();
	ledInit();
	buttonInit();
	lcdInit();
	lcdInitLayers();
	lcdSetLayer2();
	gFill(COLOR_WHITE);
	max30100Init();
	for(i=0;i<480;i++) bufIR[i]=0;
	writeIdx=0;
	max30100ResetFIFO();
	while(1){
		if(buttonRead() && showBpmSignal==0)
			showBpmSignal=1;
		else if(!buttonRead() && showBpmSignal==1)
			showBpmSignal=2;
		else if(buttonRead() && showBpmSignal==2)
			showBpmSignal=3;
		else if(!buttonRead() && showBpmSignal==3)
			showBpmSignal=0;
		if(!max30100FifoEmpty()){
			ledOn();
			max30100ReadFIFO(&rawRed,&rawIR);
			filter(UINT16_TO_FLOAT1(rawIR),&filteredIR);
			bufIR[writeIdx]=filteredIR;
			plot(writeIdx,(uint8_t)detectBPM(writeIdx),showBpmSignal==2);
			writeIdx=(writeIdx+1)%480;
			ledOff();
		}
	}
	return 1;
}
