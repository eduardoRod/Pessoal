function [t,u] = cranknicolson (odefun,tspan,y0,Nh,varargin)
%CRANKNICOLSON Resolve equacoes diferenciais com o metodo de Crank-Nicolson.
%[T, U] = CRANKNICOLSON(ODEFUN, TSPAN, Y0, NH) com TSPAN=[T0,TF] integra o
%sistema de equacoes diferenciais y'=f(t,y) do tempo T0 ao tempo TF com a
%condicao inicial Y0, usando o metodo de Crank-Nicolson com uma malha
%uniforme de NH intervalos.
%A funcao ODEFUN(T,Y) deve devolver um vector coluna correspondente a f(t,y).
%Cada linha da solucao U corresponde a um tempo do vector coluna T.

tt = linspace(tspan(1),tspan(2),Nh+1);
y = y0(:);
u = y.';
global glob_h glob_t glob_y glob_odefun;
glob_h = (tspan(2)-tspan(1))/Nh;
glob_y = y;
glob_odefun = odefun;
if (~exist('OCTAVE_VERSION'))
    options = optimset;
    options.Display = 'off';
    options.TolFun = 1.e-06;
    options.MaxFunEvals = 10000;
end

for glob_t = tt(2:end)
    if (exist('OCTAVE_VERSION'))
        [w info msg] = fzero('cranknicfun',glob_y);
    else
        w = fzero(@(w) cranknicfun(w),glob_y,options);
    end
    u=[u;w.'];
    glob_y = w;
end
t = tt;
clear glob_h glob_t glob_y glob_odefun;
return

function z = cranknicfun(w)
global glob_h glob_t glob_y glob_odefun;
z = w-glob_y-0.5*glob_h*(feval(glob_odefun,glob_t,w)+feval(glob_odefun,glob_t,glob_y));
return
