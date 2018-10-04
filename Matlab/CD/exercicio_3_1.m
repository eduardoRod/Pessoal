clear all; clc;

% variaveis globais 
% (Sistema)
global num den
% (Sinais)
global R_time R_amp D_time D_amp R D Y
% (Controlador)
global Kp Kd Ki

num = [0.5];
den = [1 2.5 1 0];
R_time = 1;
R_amp = 1;
D_time = 50;
D_amp = 1;
Kp = 3;
Ki = 0;
Kd = 0;
G = tf(num,den);

% Lugar das raizes
%rlocus(G);

% sistema malha fechada
H_r = feedback(Kp*G, 1);
H_r_zpk = zpk(H_r);

% % Pelo lugar das raizes, observa-se que a pargem
% %  de estabilidade se dá para Kp = 5
% H_crit = feedback(Kp_crit*G, 1);
% H_crit_zpk = zpk(H_crit);

% % pólos associados a (s^2 + 1) -> s1 = j, s2 = -j
% % temos (s = (phi*wn) +/- j(wd)) onde wd é freq de oscilação
% % logo, freq. de osci. para Kp = 5 é 1 rad/s
% root_den = [1 0 1];
% polos = roots(root_den);
Kp_crit = 5;
f_crit = 1;
P_crit = 2*pi/f_crit;

% segundo o segundo método de Ziegler-Nichols
Kp = 0.6 * Kp_crit;
Ti = 0.5 * P_crit;
Td = 0.125 * P_crit;
Kd = Kp * Td;
Ki = Kp / Ti;

sim('exercicio_3_1_sim.slx')

