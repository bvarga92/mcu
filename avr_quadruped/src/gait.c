#include "gait.h"
#include "servo.h"

void sitDown(void){
	uint8_t newpos[12];
	newpos[FR1]=125;
	newpos[FR2]=25;
	newpos[FR3]=25;
	newpos[FL1]=125;
	newpos[FL2]=225;
	newpos[FL3]=225;
	newpos[RR1]=125;
	newpos[RR2]=225;
	newpos[RR3]=225;
	newpos[RL1]=125;
	newpos[RL2]=25;
	newpos[RL3]=25;
	servoMoveSmooth(newpos,4,1);
}

void standUp(void){
	uint8_t newpos[12];
	newpos[FR1]=125;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=125;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,4,1);
	newpos[FR1]=210;
	newpos[RR1]=20;
	servoMoveSmooth(newpos,1,1);
}

void stepForward(void){
	uint8_t newpos[12];
	newpos[FR1]=210;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=20;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //1
	newpos[FR2]=100;
	newpos[FR3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[FR1]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[FR2]=200;
	newpos[FR3]=240;
	servoMoveSmooth(newpos,1,1); //2
	newpos[FR1]=125;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=15;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=125;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=100;
	newpos[RL2]=200;
	newpos[RL3]=240;
	servoMoveSmooth(newpos,2,1); //3
	newpos[RL2]=100;
	newpos[RL3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[RL1]=238;
	servoMoveSmooth(newpos,1,1);
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,1,1); //4
	newpos[FL2]=150;
	newpos[FL3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[FL1]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[FL2]=12;
	newpos[FL3]=12;
	servoMoveSmooth(newpos,1,1); //5
	newpos[FR1]=210;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=150;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //6
	newpos[RR2]=150;
	newpos[RR3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[RR1]=20;
	servoMoveSmooth(newpos,1,1);
	newpos[RR2]=12;
	newpos[RR3]=12;
	servoMoveSmooth(newpos,1,1); //1
}

void stepBackward(void){
	uint8_t newpos[12];
	newpos[FR1]=210;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=20;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //1
	newpos[RR2]=150;
	newpos[RR3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[RR1]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[RR2]=12;
	newpos[RR3]=12;
	servoMoveSmooth(newpos,1,1); //2
	newpos[FR1]=125;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=150;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=125;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=238;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //3
	newpos[FL2]=150;
	newpos[FL3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[FL1]=15;
	servoMoveSmooth(newpos,1,1);
	newpos[FL2]=12;
	newpos[FL3]=12;
	servoMoveSmooth(newpos,1,1); //4
	newpos[RL2]=100;
	newpos[RL3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[RL1]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[RL2]=200;
	newpos[RL3]=240;
	servoMoveSmooth(newpos,1,1); //5
	newpos[FR1]=100;
	newpos[FR2]=200;
	newpos[FR3]=240;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=20;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //6
	newpos[FR2]=100;
	newpos[FR3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[FR1]=210;
	servoMoveSmooth(newpos,1,1);
	newpos[FR2]=238;
	newpos[FR3]=238;
	servoMoveSmooth(newpos,1,1); //1
}

void turnLeft(void){
	uint8_t newpos[12];
	newpos[FR1]=210;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=20;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //1
	newpos[FR2]=100;
	newpos[FR3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[FR1]=125;
	servoMoveSmooth(newpos,1,1);
	newpos[FR2]=238;
	newpos[FR3]=238;
	servoMoveSmooth(newpos,1,1); //2
	newpos[FR1]=210;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=210;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=125;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=238;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //3
	newpos[FR2]=100;
	newpos[FR3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[FR1]=125;
	servoMoveSmooth(newpos,1,1);
	newpos[FR2]=238;
	newpos[FR3]=238;
	servoMoveSmooth(newpos,1,1); //4
	newpos[FL2]=150;
	newpos[FL3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[FL1]=125;
	servoMoveSmooth(newpos,1,1);
	newpos[FL2]=12;
	newpos[FL3]=12;
	servoMoveSmooth(newpos,1,1); //5
	newpos[RL1]=125;
	newpos[RL2]=200;
	newpos[RL3]=200;
	servoMoveSmooth(newpos,1,1);
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,1,1);
	newpos[FR1]=210;
	newpos[RR1]=20;
	servoMoveSmooth(newpos,1,1); //1
}

void turnRight(void){
	uint8_t newpos[12];
	newpos[FR1]=210;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=125;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=20;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=125;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //1
	newpos[RR2]=150;
	newpos[RR3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[RR1]=125;
	servoMoveSmooth(newpos,1,1);
	newpos[RR2]=12;
	newpos[RR3]=12;
	servoMoveSmooth(newpos,1,1); //2
	newpos[FR1]=125;
	newpos[FR2]=238;
	newpos[FR3]=238;
	newpos[FL1]=20;
	newpos[FL2]=12;
	newpos[FL3]=12;
	newpos[RR1]=20;
	newpos[RR2]=12;
	newpos[RR3]=12;
	newpos[RL1]=12;
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,2,1); //3
	newpos[RR2]=150;
	newpos[RR3]=150;
	servoMoveSmooth(newpos,1,1);
	newpos[RR1]=125;
	servoMoveSmooth(newpos,1,1);
	newpos[RR2]=12;
	newpos[RR3]=12;
	servoMoveSmooth(newpos,1,1); //4
	newpos[RL2]=100;
	newpos[RL3]=100;
	servoMoveSmooth(newpos,1,1);
	newpos[RL1]=125;
	servoMoveSmooth(newpos,1,1);
	newpos[RL2]=238;
	newpos[RL3]=238;
	servoMoveSmooth(newpos,1,1); //5
	newpos[FL1]=125;
	newpos[FL2]=50;
	newpos[FL3]=50;
	servoMoveSmooth(newpos,1,1);
	newpos[FL2]=12;
	newpos[FL3]=12;
	servoMoveSmooth(newpos,1,1);
	newpos[FR1]=210;
	newpos[RR1]=20;
	servoMoveSmooth(newpos,1,1); //1
}
