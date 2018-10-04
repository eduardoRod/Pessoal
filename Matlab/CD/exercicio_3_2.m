clear all; clc;

% variaveis globais 
% (Sistema)
global num den
% (Sinais)
global R_time R_amp D_time D_amp R D D1 Y Y1
% (Controlador)
global Kp Kd Ki

num = [2.05];
den = [1 3.74 0];
R_time = 1;
R_amp = 1;
D_time = 10;
D_amp = 1;

G = tf(num,den);
b0 = num(1);
a0 = den(3);
a1 = den(2);
phi = 0.5;
wn = 2;

% lugar de raizes
%rlocus(G)

% parte real dos pólos conjugados
Re = -phi*wn;
p = -10*Re;

Kp = (2*phi*wn*p + wn^2 - a0)/b0;
Ki = (p*wn^2)/b0;
Kd = (p + 2*phi*wn - a1)/b0;

sim('exercicio_3_1_sim.slx');
subplot(2, 1, 1);
plot(R.time, R.signals.values, 'r',...
    D.time, D.signals.values, 'g', ...
    Y.time, Y.signals.values, 'b');
subplot(2, 1, 2);
plot(R.time, R.signals.values, 'r',...
    D1.time, D1.signals.values, 'g', ...
    Y1.time, Y1.signals.values, 'b');