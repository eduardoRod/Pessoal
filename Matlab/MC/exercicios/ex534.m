clear all; clc;

altura = [1500 1250 1000 750 500];
distancia = [35 25 15 10 7];
xi = 850;

p = polyfit(altura,distancia,4);
x = 500:0.5:1500;
% p = polyfit(altura,distancia,2);
% x = 0:0.5:2000;
y = polyval(p,x);
yi = polyval(p,xi);

plot(altura,distancia,'*',x,y,xi,yi,'r*');
shg;