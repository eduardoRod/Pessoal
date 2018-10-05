function [A] = trocaLinhas(A,i,j) 
x=A(i,:);
A(i,:) = A(j,:);
A(j,:) = x;
return