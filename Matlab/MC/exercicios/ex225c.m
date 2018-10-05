clear all; close all; clc;

%dados de enunciado
n = 15;
xexact = [1;1;1];
ep = 10.^(-(1:n));
condA = zeros(n,1);

%calculado por enunciado (b = A*x)
for i = 1;n;
    A = [2 -2 0; ep(i)-2 2 0; 0 -1 3];
    [L,U] = lu(A);
    b = A*xexact;
    y = L\b;
    x = U\y;
    bCalc = L*U*x;
    condA(i) = cond(A);
    erroRelx(i) = norm(xexact - x)/norm(xexact);
    erroRelb(i) = norm(b - bCalc)/norm(b);
end

subplot(2,1,1);
plot(i,erroRelx,i,erroRelb);
% subplot(2,1,2);
% loglog(i,condA);