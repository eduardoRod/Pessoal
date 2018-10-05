function [x,niter,erro]=pontofixo(phi,x0,tol,nmax,varargin)
% PONTOFIXO Metodo do Ponto Fixo
% [x,niter,erro]=pontofixo(phi,x0,tol,nmax) aplica o metodo do ponto fixo para aproximar
% um ponto fixo X da funcao PHI, usando como aproximacao inicial x0. 
% A funcao pontofixo(phi,x0,tol,nmax) devolve uma aproximacao do ponto fixo, o numero de 
% iteracoes efectuadas e uma estimativa para o erro cometido.

if nargin==2
    tol=1.e-4; nmax=100;
elseif nargin==3
    nmax=100;
end
x=x0;
diff=tol+1;
niter=0;
while niter<=nmax && diff>=tol
    gx=feval (phi,x,varargin {:});
    xnew=gx;
    diff=abs (x-xnew);
    x=xnew;
    erro=diff;
    niter=niter+1;
end
if niter>nmax
    fprintf ('Não converge no número máximo de iterações \n ');
end
return