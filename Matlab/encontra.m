function [valores,indice,Nvalores]=encontra(min,max,vector);

valores=[]; indice=[];

clc;
for i=1:1:length(vector);
    if (vector(i)>=min && vector(i)<=max)
        valores=[valores,vector(i)];
        indice=[indice,i];
        fprintf('\n::O valor "%g" que está na posição "%d" do vector está no intervalo ["%g","%g"]',vector(i),i,min,max);
    end
end
Nvalores=length(valores);

fprintf('\n\n::Foram encontrados "%g" valores no intervalo ["%g","%g"]',Nvalores,min,max);

return