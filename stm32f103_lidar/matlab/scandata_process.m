clear all;
clc;

R=15;       % sorok szama
C=30;       % oszlopok szama
blur=false; % szurjuk-e meg a kepet?
N=10;       % interpolacios faktor

data=load('scandata.txt');
data=reshape(data,C,R)';
data(2:2:size(data,1),:)=data(2:2:size(data,1),end:-1:1);
data=fliplr(data);
if blur
    data=conv2(data,[1 2 1;2 4 2;1 2 1]/16);
end
[xi,yi]=meshgrid(linspace(1,size(data,2),N*size(data,2)),linspace(1,size(data,1),N*size(data,1)));
data=interp2(data,xi,yi,'linear');
figure(1);
imagesc(data);
axis equal off;
