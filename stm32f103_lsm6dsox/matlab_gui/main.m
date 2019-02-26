clc;
clear all;

com=serial('COM15');
com.BytesAvailableFcnMode='terminator';
com.Terminator='$';
com.BytesAvailableFcn=@recvCallback;
fopen(com);

pause;

fclose(com);
delete(com);