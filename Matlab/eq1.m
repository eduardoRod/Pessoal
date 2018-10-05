function dydt = sistema(t,y)

y=y(1);
x=y(2);
dxdt=y;
dydt=-0.5*y-9.85*sin(x);

