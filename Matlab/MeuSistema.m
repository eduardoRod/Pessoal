function dydt=MeuSistema(t,y)

f=y(1);
g=y(2);

dfdt=f-4*g; % primeira equa��o diferencial
dgdt=-f+g; % segunda equa��o diferencial
dydt=[dfdt; dgdt]; % � for�oso que dydt seja um vector coluna