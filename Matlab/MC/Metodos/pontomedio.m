function Imp=pontomedio(a,b,M,f,varargin)
% PONTO MEDIO formula de integracao numerica do ponto medio
% Imp = pontomedio(a,b,M,f) calcula uma aproximacao para o integral \int_a^b f(x) dx usando a regra do ponto
% medio com M subintervalos.
H=(b-a)/M;
x=linspace (a+H/2,b-H/2,M);
fmp=feval (f,x,varargin {:}).*ones (1,M);
Imp=H*sum (fmp);
return