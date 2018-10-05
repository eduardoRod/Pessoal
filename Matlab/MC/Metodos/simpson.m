function Isic=simpson(a,b,M,f,varargin)
% SIMPSON formula de integracao numerica de Simpson
% Isic = simpson(a,b,M,f) calcula uma aproximacao para o integral \int_a^b f(x) dx usando a regra de Simpson
% com M subintervalos.
H=(b-a)/M;
x=linspace (a,b,M+1);
fpm=feval (f,x,varargin {:}).*ones (1,M+1);
fpm (2:end-1)=2*fpm (2:end-1);
Isic=H*sum (fpm)/6;
x=linspace (a+H/2,b-H/2,M);
fpm=feval (f,x,varargin {:}).*ones (1,M);
Isic=Isic+2*H*sum (fpm)/3;
return