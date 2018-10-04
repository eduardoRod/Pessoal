% H(s) = 10(s+3) / s^3 + 7s^2 + 16s + 30;

% cálculo de pólos
den = [1 7 16 30];
polos = roots(den);

% polos =
% 
%   -5.0000 + 0.0000i
%   -1.0000 + 2.2361i
%   -1.0000 - 2.2361i

% H(s) = 10(s+3) / (s+5)(s+p2)(s+*p2);

syms s

p1 = -1.0000 + 2.2361i;
p2 = -1.0000 - 2.2361i;

eq = (s-p1)*(s-p2);
eq = simplify(eq);

% H(s) = 10(s+3) / (s+5)(s^2 + 2s + 6);