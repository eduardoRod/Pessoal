function [zero,niter,erro] = myNewton(f,df,x0,maxniter,tol)
%%function [zero,niter,erro] = myNewton(f,df,x0,maxniter,tol)

niter = 0;
zero = x0;
erro = x0/norm(x0);

while erro >= tol && niter < maxniter
    niter = niter + 1;
    newZero = zero - (feval(f,zero)/feval(df,zero));
    erro = norm(zero-newZero);
    zero = newZero;
end