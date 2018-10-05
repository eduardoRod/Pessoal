clear all; clc;

f = inline('x + 0.5 + 2*cos(pi*x)','x');
eixox = inline('0','x');
df = inline('1 - 2*pi*sin(pi*x)','x');

a = 0.5; b = 1;
fplot(f, [a b]); hold on; fplot(eixox, [a b], 'r'); grid on;

tol = 10^-2;
x0 = 0.7;
maxiter = 200;

[zero1,niter1,erro1] = myNewton(f,df,x0,maxiter,tol)
plot(zero1,f(zero1),'*'); 
shg; hold off;