clear all; clc;

f = inline('sin(x) - sqrt((2*9.8)/100)','x');
df = inline('cos(x)','x');
a = 0; b = pi/2;
fplot(f,[a b]); hold on;  grid on;
%%eixox = inline('0','x');
%%fplot(eixox, [a b], 'r');
x0 = 0.5;
maxniter = 200;
tol = 10^-5;

tic;
[zero,niter,erro] = myNewton(f,df,x0,maxniter,tol);
toc;

tic;
[zero,res,niter,erro]=bisseccao(f,a,b,tol,maxniter);
toc;

tic;
[zero,res,niter,erro]=newton(f,df,x0,tol,maxniter);
toc;

plot(zero,f(zero),'*'); 
shg; hold off;
