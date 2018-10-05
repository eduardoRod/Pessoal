function [P,L,A] = factorizacaoPALR(A)
[n,m]=size(A);
L = eye(n,m);
P = eye(n,m);
if (n ~= m), error('Apenas para matrizes quadradas'); end
for k=1:n-1
  [C,I] = max(abs(A(k:end,k)));
  indice = k+I(1)-1;

  A = trocaLinhas(A,k,indice); P = trocaLinhas(P,k,indice);
  L(:,1:k-1) = trocaLinhas(L(:,1:k-1),k,indice);

  for i=(k+1):n
      L(i,k) = A(i,k)/A(k,k);
      A(i,1:n) = A(i,1:n) - L(i,k)*A(k,1:n);
  end
end
return