function [x,F,iter,erro]=newtonsys(Ffun,Jfun,x0,tol,nmax,varargin)
% NEWTONSYS Metodo de Newton para Sistemas Nao Lineares
% [x,F,iter,erro]=newtonsys(Ffun,Jfun,x0,tol,nmax) aplica o metodo de Newton
% para aproximar um zero X da funcao Ffun, usando como aproximacao inicial x0 e a matriz de Jacobi Jfun. 
% A funcao newtonsys(Ffun,Jfun,x0,tol,nmax) devolve uma aproximacao do zero X, o residuo F(X), o numero de 
% iteracoes efectuadas e uma estimativa para o erro cometido.

iter=0; erro=tol+1; x=x0;
while erro>tol && iter<nmax
    J=feval (Jfun,x,varargin {:});
    F=feval (Ffun,x,varargin {:});
    delta=-J\F;
    x=x+delta;
    erro=norm (delta);
    iter=iter+1;
end
F=norm (feval (Ffun,x,varargin {:}));
if iter>=nmax
    fprintf ('Nao converge com o numero maximo de iteracoes \n ');
    fprintf ('O valor devolvido tem um residuo relativo % e \n',F);
else
    fprintf ('O metodo convergiu na iteracao % i com um residuo % e \n',iter,F);
end
return