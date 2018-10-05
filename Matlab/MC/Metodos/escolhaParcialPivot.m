function [A,b] = escolhaParcialPivot(A,b,p) % p denota o indice da coluna onde vamos procurar o maior elemento (em valor absoluto)

[C,I] = max(abs(A(p:end,p)));

x = A(p,p:end);
A(p,p:end) = A(p+I(1)-1,p:end);
A(p+I(1)-1,p:end) = x;

y = b(p);
b(p) = b(p+I(1)-1);
b(p+I(1)-1) = y;
return