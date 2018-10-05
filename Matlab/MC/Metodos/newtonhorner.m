function [roots,iter]=newtonhorner(a,x0,tol,nmax)
% NEWTONHORNER Metodo de Newton-Horner
% [roots,iter]=newtonhorner(a,x0,tol,nmax) aplica o metodo de Newton-Horner para aproximar
% os zeros do polinomio p(x) = a(1)*x^n + a(2)*x^(n-1) + ... + a(n-1)*x + a(n).
% A funcao tem como output o conjunto dos zeros reais e o numero de iteracoes necessarias pelo
% metodo de Newton para a aproximacao de cada zero.

if nargin==2
    tol=1.e-04; nmax=100;
elseif nargin==3
    nmax=100;
end
n=length(a)-1; roots=zeros(n,1); iter=zeros(n,1);
for k=1:n
    niter=0; x=x0; diff=tol+1;
    while niter<=nmax && diff>=tol
        [pz,b]=horner(a,x); [dpz,b]=horner(b,x);
        xnew=x-pz/dpz;   diff=abs(xnew-x);
        niter=niter+1;   x=xnew;
    end
    if niter>=nmax
        fprintf('Nao converge no numero maximo de iteracoes \n ')
    end
    %Deflaction
    [pz,a]=horner(a,x); roots(k)=x; iter(k)=niter;
end
return