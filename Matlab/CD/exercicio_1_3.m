% ex. 1.3

clear, clc;
num = [2 6];
den = [1 7 6 0];
K = 5;
H = 1;

% funça0 transferencia
G = tf(num,den);
Gzpk = zpk(G);

% lugar da raizes
rlocus(G);

% sistema de malha fechada
H_r = feedback(K*G, H);
% rlocus(H_r)
% return

% resposta a degrau
step(H_r);
%return

% com T(s) != 1
num_t = [3];
den_t = [1 3];
T = tf(num_t, den_t);

% sistema T(s)*H_r(s)
TH_r = series(T, H_r);