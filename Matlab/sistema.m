function dy = sistema(t,g)

x=g(1);
y=g(2);

dxdt=y;
dydt=-0.5*y-9.85*sin(x);

dy=[dxdt; dydt];