function [x,iter]=metodoiterativo (A,b,x0,nmax,tol,P)
% METODOITERATIVO Metodo iterativo de Jacobi ou Gauss-Seidel
% [x,iter] = metodoiterativo (A,b,x0,nmax,tol,P) calcula a solucao aproximada do sistema Ax=b, partindo da aproximacao
% inicial x0. Apos ITER iteracoes, o metodo devolve a solucao X.
% Para utilizar o metodo de Jacobi devera indicar P='J'. Caso pretenda utilizar o metodo de Gauss-Seidel, devera
% utilizar P='G'

[n,n]=size (A);
if nargin==6
    if ischar (P)==1
        if P=='J'
            L=diag (diag (A));
            U=eye (n);
            beta=1;
            alpha=1;elseif P=='G'
            L=tril (A);
            U=eye (n);
            beta=1;
            alpha=1;
        end
    else
        [L,U]=lu (P);
        beta=0;
    end
else
    L=eye (n);
    U=L;
    beta=0;
end
iter=0;
r=b-A*x0;
r0=norm (r);
err=norm (r);
x=x0;
while err>tol && iter<nmax
    iter=iter+1;
    z=L \r;
    z=U\z;
    if beta==0
        alpha=z'*r/(z'*A*z);
    end
    x=x+alpha*z;
    r=b-A*x;
    err=norm (r)/r0;
end
            