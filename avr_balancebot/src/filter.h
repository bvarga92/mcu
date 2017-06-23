#ifndef _FILTER_H_
#define _FILTER_H_

	/* ekkora sullyal veszi figyelembe a komplementer szuro a gyorsulasmero alapjan szamolt szoget */
	#define ACC_WEIGHT 0.03f

	/* komplementer szuro */
	float filterComplementary(float tilt, float accDeg, float gyrDegPerSec, float sampleTime);

/*-----------------------------------------------------------------------------------------------------------------------------*/

	/* Kalman-szuro parameterek */
	#define Q1 10.0f   //modellzaj variancia szoghoz
	#define Q2 100.0f  //modellzaj variancia szogsebesseghez
	#define Q3 0.01f   //modellzaj variancia szogsebesseg ofszethez
	#define R1 8000.0f //gyorsulasmero variancia
	#define R2 1000.0f //gyro variancia

	/* adatstruktura a Kalman-szuro futasanak nyilvantartasahoz */
	typedef struct{
		float x1, x2, x3; //a harom allapot aktualis erteke (szog, szogsebesseg, szogsebesseg ofszet)
		float p11, p12, p13, p21, p22, p23, p31, p32, p33; //a P matrix elemei (mennyire megbizhatoak az aktualis allapotok)
	} kalman_t;

	/* Kalman-szuro inicializalasa */
	void filterKalmanInit(kalman_t *kalman);

	/* lefuttat egy utemet a Kalman-szuron */
	float filterKalmanUpdate(kalman_t *kalman, float accDeg, float accDegPerSec, float sampleTime);

#endif
