function [zero,res,niter,erro]=newton(f,df,x0,tol,nmax,varargin)
% NEWTON Metodo de Newton
% [zero,res,niter,erro]=newton(f,df,x0,tol,nmax) aplica o metodo de Newton para aproximar
% um zero ZERO da funcao diferenciavel F (com derivada DF) com aproximacao inicial x0. 
% A funcao newton(f,df,x0,tol,nmax) devolve o residuo em ZERO da funcao F, o numero de 
% iteracoes efectuadas e uma estimativa para o erro cometido.

x=x0;
fx=feval (f,x,varargin {:});
dfx=feval (df,x,varargin {:});
niter=0;
er=tol*10;

while er>=tol && niter<=nmax
    niter=niter+1;      diff=-fx/dfx;
    xold=x;             
    x=x+diff;           diff=abs (diff);
    fx=feval (f,x,varargin {:});
    dfx=feval (df,x,varargin {:});
    er=abs (x-xold);
end
zero=x; res=fx; erro=er;
return