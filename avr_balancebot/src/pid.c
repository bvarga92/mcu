#include "pid.h"

void pidCreate(pid_t* pid, float* setpoint, float* input, float* output, float K, float Kp, float Ki, float Kd, float minOut, float maxOut){
	pid->setpoint=setpoint;
	pid->input=input;
	pid->output=output;
	pid->error=*setpoint-*input;
	pid->errorIntegral=0;
	pid->K=K;
	pid->Kp=Kp;
	pid->Ki=Ki;
	pid->Kd=Kd;
	pid->minOut=minOut;
	pid->maxOut=maxOut;
}

void pidUpdate(pid_t* pid){
	float pTerm, iTerm, dTerm, out;
	float prevError=pid->error;
	pid->error=*(pid->setpoint)-*(pid->input);
	pid->errorIntegral+=pid->error;
	if((pid->errorIntegral)>MAX_INT)
		pid->errorIntegral=MAX_INT;
	else if((pid->errorIntegral)<-MAX_INT)
		pid->errorIntegral=-MAX_INT;
	pTerm=(pid->Kp)*(pid->error);                              
	iTerm=(pid->Ki)*(pid->errorIntegral);
	dTerm=(pid->Kd)*((pid->error)-prevError);
	out=(pid->K)*(pTerm+iTerm+dTerm);
	if(out>(pid->maxOut))
		*(pid->output)=pid->maxOut;
	else if(out<(pid->minOut))
		*(pid->output)=pid->minOut;
	else
		*(pid->output)=out;
}
