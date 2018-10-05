clear all; close all; clc;

%dados de enunciado
n = 15;
ep = 10.^(-(1:n));
condA = zeros(n,1);

%calculado por enunciado (b = A*x)
for i = 1;n;
    A = [2 -2 0; ep(i)-2 2 0; 0 -1 3];
    condA(i) = cond(A);
end

subplot(2,1,1);
plot(ep,condA);
subplot(2,1,2);
loglog(ep,condA);