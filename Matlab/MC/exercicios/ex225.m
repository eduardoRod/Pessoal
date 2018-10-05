clear all; close all; clc;

%dados de enunciado
ep = 0.1;
xexacto = [1;1;1];
A = [2 -2 0; ep-2 2 0; 0 -1 3];
%calculado por enunciado (b = A*x)
b = [0; ep; 2];

%factorização sem escolha
[L,U] = factorizacaoLU(A);
y = L\b;
x = U\y;
erroSem = norm(A*x-b)

%factorização com escolha
[L,U] = lu(A);
y = L\b;
x = U\y;
erroCom = norm(A*x-b)

eps
clear all;