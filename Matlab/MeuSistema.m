function dydt=MeuSistema(t,y)

f=y(1);
g=y(2);

dfdt=f-4*g; % primeira equação diferencial
dgdt=-f+g; % segunda equação diferencial
dydt=[dfdt; dgdt]; % é forçoso que dydt seja um vector coluna