function [Valores,Indices,NValores]=EncontraNoIntervalo(min,max,vector);
%Valores, Indices e NValores, são o output da função
%min, max e vector, são os parâmetros de entrada

%inicialização das variáveis de output
Valores=[]; Indices=[];
%limpa o ecrã
clc;
%circlo for (duh!)... com uma variável 'i' que varia, em passos de '1', de '1' até 'length(vector' (comprimento do 'vector' de entrada)
for i=1:1:length(vector);
    %se o valor na posição 'i' do 'vector' de entrada for maior ou igual a 'min' e menor ou igual a 'max'...
    if ( vector(i)>=min && vector(i)<=max );
        %...coloca esse valor (que está na posição 'i' do 'vector') no array 'Valores'
        Valores=[Valores,vector(i)];
        %...coloca o indice desse valor (relativamente ao 'vector') no array 'Indices'
        Indices=[Indices,i];
        %...escreve para o ecrã (...)
        fprintf('\n::: O valor "%g" encontra-se na posição "%d" do vector, e pertence ao intervalo [%g,%g]',vector(i),i,min,max);
    end
end
%'NValores' fica com o numero de elementos encontrados no intervalo, que é o comprimento de 'Valores'
NValores=length(Valores);
%escreve no ecrã (...)
fprintf('\n\n::: Foram encontrados %d valores pertencentes ao intervalo [%g,%g]\n\n',NValores,min,max);
%sair da função
return