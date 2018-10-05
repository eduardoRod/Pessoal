clear all; clc;

f = inline('log(x)','x');
df = inline('1/x','x');
x0 = 0.1;
tol = 10e-3;
maxiter = 200;

[zero,niter,erro] = myNewton(f,df,x0,maxiter,tol)