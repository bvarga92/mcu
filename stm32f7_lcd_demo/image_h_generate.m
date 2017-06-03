clear all;
clc;
I=imread('image.bmp');
r=double(bitshift(I(:,:,1)',-3,8));
g=double(bitshift(I(:,:,2)',-2,8));
b=double(bitshift(I(:,:,3)',-3,8));
x=r*2^11+g*2^5+b;
file=fopen('image.h','w');
fprintf(file,'#ifndef _IMAGE_H_\n#define _IMAGE_H_\n\n');
fprintf(file,'const uint16_t bmp[]={\n');
fprintf(file,'0x%04X,\n',x(1:end-1));
fprintf(file,'0x%04X\n};\n\n',x(end));
fprintf(file,'image_t img={bmp,%d,%d};\n\n#endif\n',size(I,2),size(I,1));
fclose(file);
