function [zero,res,niter,erro]=bisseccao(f,a,b,tol,nmax,varargin)
% BISSECCAO Metodo da Bisseccao
% [zero,res,niter,erro]=bisseccao(f,a,b,tol,nmax) aplica o metodo da bisseccao para aproximar
% um zero ZERO da funcao continua F no intervalo [A,B]. A funcao bisseccao(f,a,b,tol,nmax) 
% devolve o residuo em ZERO da função F, o número de iteracoes efectuadas e uma estimativa 
% para o erro cometido.

x=[a (a+b)*0.5 b]; fx = feval(f,x,varargin{:});
if fx(1)*fx(3)>0
    error('Os sinais nos extremos do intervalo devem ser diferentes');
elseif fx(1)==0
    zero=a; res=0; niter=0; return
elseif fx(3)==0
    zero=b; res=0; niter=0; return
end
niter=0;
I=(b-a)/2;
while I>=tol && niter<=(nmax-1)
    niter=niter+1;
    if fx(1)*fx(2)<0
        x(3)=x(2); x(2)=x(1)+(x(3)-x(1))*0.5;
        fx=feval(f,x);  I=(x(3)-x(1))*0.5;
    elseif fx(2)*fx(3)<0
        x(1)=x(2); x(2)=x(1)+(x(3)-x(1))*0.5;
        fx = feval(f,x,varargin{:}); I=(x(3)-x(1))*0.5;
    else 
        x(2)=x(find(fx==0)); I=0; 
    end
end
if niter > nmax 
    fprintf(['Foi ultrapassado o numero maximo de iteracoes \n']);
end
zero=x(2); x=x(2); res = feval(f,x,varargin{:}); erro=I;
return
    