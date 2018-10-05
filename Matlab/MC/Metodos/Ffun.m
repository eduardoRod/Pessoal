function F=fun(x)

F=zeros(2,1);
F(1,1)=x(1)^2 + x(2)^2 -1;
F(2,1)=x(1)*x(2)+x(1) +1;

return;