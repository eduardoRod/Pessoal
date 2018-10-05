clear all; clc;
a = -5; b = 5;


x = a:0.01:b;
xi = linspace(x(1),x(end),7);
yi = 1./(1+xi.^2);
y = 1./(1+x.^2);
subplot(2,1,1);
plot(xi,yi,'*g',x,y,'r');
subplot(2,1,2);
plot(x,y,'r');

subplot(2,1,1); hold on;
for n=7:2:14
    xi = linspace(x(1),x(end),n);
    yi = 1./(1+xi.^2);
    p = polyfit(xi,yi,n);
    y = polyval(p,x);
    plot(x,y);
end
hold off;

subplot(2,1,2);
for n=7:2:20
    xtemp = zeros(1,n);
    for i = 1:n
        xtemp(i) = cos((2*i-1)*pi/(2*n));
    end
    xi = (a+b)*0.5 + (b-a)*xtemp*0.5;
    yi = 1./(1+xi.^2);
    plot(xi,yi,'*g');
    p = polyfit(xi,yi,n);
    y = polyval(p,x);
    plot(x,y);
    shg;
end

shg;