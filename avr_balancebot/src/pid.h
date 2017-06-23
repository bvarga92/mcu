#ifndef _PID_H_
#define _PID_H_

	typedef struct{
		float* setpoint;      //alapjel
		float* input;         //bemeneti jel
		float* output;        //kimeneti jel
		float error;          //hibajel
		float errorIntegral;  //a hibajel integralja
		float K, Kp, Ki, Kd;  //szabalyzo parameterei
		float minOut, maxOut; //kimeneti jel korlatai
	} pid_t;

	/* az integralt hiba abszoluterteket ennel nem engedjuk nagyobbra noni */
	#define MAX_INT 200

	/* letrehoz es visszaad egy szabalyzot */
	void pidCreate(pid_t* pid, float* setpoint, float* input, float* output, float K, float Kp, float Ki, float Kd, float minOut, float maxOut);

	/* az atadott szabalyzon vegrehajt egy utemet */
	void pidUpdate(pid_t* pid);

#endif
