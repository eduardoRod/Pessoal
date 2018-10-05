function [t,u] = eulerprog(odefun,tspan,u,Nh,varargin)
%EULERPROG Resolve equacoes diferenciais com o metodo de Euler progressivo.
%[T, U] = EULERPROG(ODEFUN, TSPAN, Y0, NH) com TSPAN=[T0,TF] integra o
%sistema de equacoes diferenciais y'=f(t,y) do tempo T0 ao tempo TF com a
%condicao inicial Y0, usando o metodo de Euler progressivo com uma malha
%uniforme de NH intervalos.
%A funcao ODEFUN(T,Y) deve devolver um vector coluna correspondente a f(t,y).
%Cada linha da solucao U corresponde a um tempo do vector coluna T.

h = (tspan(2)-tspan(1))/Nh;
tt = linspace (tspan(1),tspan(2),Nh+1);
for t = tt(1:end-1)
    u = [u;u(end,:)+h*feval(odefun,t,u(end,:),varargin{:})];
end
t=tt;
return
