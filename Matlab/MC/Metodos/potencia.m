function [lambda,x,iter]=potencia(A,tol,nmax,x0)
% POTENCIA Metodo da Potencia
% [lambda,x,iter]=potencia (A,tol,nmax,x0) calcula o valor proprio de maior modulo da matriz A. O algoritmo 
% necessita de uma aproximação inicial x0 e para quando o erro relativo for menor que TOL ou o numero de 
% iteracoes atingir NMAX.

[n,m]=size(A);
if n~=m, error('So para matrizes quadradas'); end
if nargin==1
    tol=1.e-06;
    x0=ones(n,1);
    nmax=100;
end
x0=x0/norm(x0);
pro=A*x0;
lambda=x0'*pro;
err=tol*abs(lambda)+1;
iter=0;
while err>tol*abs(lambda) && abs(lambda)~=0 && iter<=nmax
    x=pro; x=x/norm(x); 
    pro=A*x; lambdanew=x'*pro;
    err=abs(lambdanew-lambda);
    lambda=lambdanew;
    iter=iter+1;
end
