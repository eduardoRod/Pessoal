function [L,A] = factorizacaoLU(A)
[n,m]=size(A);
L = eye(n,m);
if (n ~= m), error('Apenas para matrizes quadradas'); end
for k=1:n-1
  if (A(k,k) == 0), error('Pivot nulo. E necessaria troca de linhas'); end
  for i=(k+1):n
      L(i,k) = A(i,k)/A(k,k);
      A(i,1:n) = A(i,1:n) - L(i,k)*A(k,1:n);
  end
end
return