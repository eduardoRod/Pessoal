clear all; clc; close all;

n = 10;
a = 10.^(1:n);
k = zeros(n,1);

for i = 1:n
    A = [1 0 a(i); 0 1 0; -a(i) 0 1];
    k(i) = cond(A,inf);
end

subplot(2,1,1);
plot(a,k);
subplot(2,1,2);
loglog(a,k);
shg;