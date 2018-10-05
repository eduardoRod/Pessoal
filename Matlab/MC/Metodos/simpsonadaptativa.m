function [ISf,nodes]=simpsonadaptativa(f,a,b,tol,hmin,varargin)
% SIMPSON ADAPTATIVA formula de integracao numerica de Simpson adaptativa
% [ISf,nodes] = simpsonadaptativa(f,a,b,tol,hmin) calcula uma aproximacao para o integral \int_a^b f(x) dx usando a regra de Simpson
% numa malha de pontos nao uniforme. O espacamento minimo aceite pela malha e HMIN e o integral e aproximado com uma tolerancia TOL.
A=[a,b]; N=[]; S=[]; ISf=0; ba=b-a; nodes=[];
while ~isempty(A),
    [deltaI,ISc]=caldeltai(A,f,varargin{:});
    if abs(deltaI) <= 15*tol*(A(2)-A(1))/ba;
        ISf=ISf+ISc; S=union(S,A);
        nodes=[nodes,  A(1) (A(1)+A(2))*0.5 A(2)];
        S=[S(1),S(end)]; A=N; N=[];
    elseif A(2)-A(1) < hmin
        ISf=ISf+ISc;  S=union(S,A);
        S=[S(1),S(end)]; A=N; N=[];
        warning('Passo de integracao demasiado pequeno');
    else
        Am=(A(1)+A(2))*0.5;
        A=[A(1) Am];
        N=[Am, b];
    end
end
nodes=unique(nodes);
return

function [deltaI,ISc]=caldeltai(A,f,varargin)
L=A(2)-A(1);
t=[0; 0.25; 0.5; 0.5; 0.75; 1];
x=L*t+A(1);
L=L/6;
w=[1; 4; 1];
fx=feval(f,x,varargin{:}).*ones(6,1);
IS=L*sum(fx([1 3 6]).*w);
ISc=0.5*L*sum(fx.*[w;w]);
deltaI=IS-ISc;
return
        