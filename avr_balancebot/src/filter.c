#include "filter.h"

float filterComplementary(float tilt, float accDeg, float gyrDegPerSec, float sampleTime){
	return ACC_WEIGHT*accDeg+(1-ACC_WEIGHT)*(gyrDegPerSec*sampleTime+tilt);
}

void filterKalmanInit(kalman_t *kalman){
	/* kezdeti allapotokat valasszuk nullanak */
	kalman->x1=0;
	kalman->x2=0;
	kalman->x3=0;
	/* de ez valoszinuleg nem igy van, ugyhogy P kezdeti ertekei nagyok */
	kalman->p11=1000;
	kalman->p12=0;
	kalman->p13=0;
	kalman->p21=0;
	kalman->p22=1000;
	kalman->p23=0;
	kalman->p31=0;
	kalman->p32=0;
	kalman->p33=1000;
}

float filterKalmanUpdate(kalman_t *kalman, float accDeg, float accDegPerSec, float sampleTime){
	float y1, y2;
	float a, b, c;
	float sDet;
	float s11, s12, s21, s22;
	float k11, k12, k21, k22, k31, k32;
	float p11, p12, p13, p21, p22, p23, p31, p32, p33;
	/* allapotvektor frissitese a modell alapjan: x=F*x+w */
	kalman->x1=kalman->x1+sampleTime*kalman->x2-sampleTime*kalman->x3;
	/* kovarianciamatrix a modell alapjan: P=F*P*F'+Q */
	a=kalman->p11+kalman->p21*sampleTime-kalman->p31*sampleTime;
	b=kalman->p12+kalman->p22*sampleTime-kalman->p32*sampleTime;
	c=kalman->p13+kalman->p23*sampleTime-kalman->p33*sampleTime;
	kalman->p11=a+b*sampleTime-c*sampleTime+Q1;
	kalman->p12=b;
	kalman->p13=c;
	kalman->p21=kalman->p21+kalman->p22*sampleTime-kalman->p23*sampleTime;
	kalman->p22=kalman->p22+Q2;
	kalman->p31=kalman->p31+kalman->p32*sampleTime-kalman->p33*sampleTime;
	kalman->p33=kalman->p33+Q3; 
	/* szamitott es mert allapot kozti elteres: y=z-H*x */
	y1=accDeg-kalman->x1;
	y2=accDegPerSec-kalman->x2;
	/* kovarianciamatrix a meres alapjan: S=H*P*T'+R */
	s11=kalman->p11+R1;
	s12=kalman->p12;
	s21=kalman->p21;
	s22=kalman->p22+R2;
	/* Kalman-erosites: K=P*H*inv(S) */
	sDet=1.0f/(s11*s22-s12*s21);
	k11=(kalman->p11*s22-kalman->p12*s21)*sDet;
	k12=(kalman->p12*s11-kalman->p11*s12)*sDet;
	k21=(kalman->p21*s22-kalman->p22*s21)*sDet;
	k22=(kalman->p22*s11-kalman->p21*s12)*sDet;
	k31=(kalman->p31*s22-kalman->p32*s21)*sDet;
	k32=(kalman->p32*s11-kalman->p31*s12)*sDet; 
	/* allapotvektor frissitese: x=x+K*y */
	kalman->x1=kalman->x1+k11*y1+k12*y2;
	kalman->x2=kalman->x2+k21*y1+k22*y2;
	kalman->x3=kalman->x3+k31*y1+k32*y2;
	/* kovarianciamatrix frissitese: P=(I-K*H)*P */
	p11=kalman->p11*(1.0f-k11)-kalman->p21*k12;
	p12=kalman->p12*(1.0f-k11)-kalman->p22*k12;
	p13=kalman->p13*(1.0f-k11)-kalman->p23*k12;
	p21=kalman->p21*(1.0f-k22)-kalman->p11*k21;
	p22=kalman->p22*(1.0f-k22)-kalman->p12*k21;
	p23=kalman->p23*(1.0f-k22)-kalman->p13*k21;
	p31=kalman->p31-kalman->p21*k32-kalman->p11*k31;
	p32=kalman->p32-kalman->p22*k32-kalman->p12*k31;
	p33=kalman->p33-kalman->p23*k32-kalman->p13*k31;
	kalman->p11=p11; kalman->p12=p12; kalman->p13=p13;
	kalman->p21=p21; kalman->p22=p22; kalman->p23=p23;
	kalman->p31=p31; kalman->p32=p32; kalman->p33=p33;
	/* a dolesszog az elso allapotvaltozo */
	return kalman->x1;
}
