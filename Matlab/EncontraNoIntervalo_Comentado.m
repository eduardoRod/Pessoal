function [Valores,Indices,NValores]=EncontraNoIntervalo(min,max,vector);
%Valores, Indices e NValores, s�o o output da fun��o
%min, max e vector, s�o os par�metros de entrada

%inicializa��o das vari�veis de output
Valores=[]; Indices=[];
%limpa o ecr�
clc;
%circlo for (duh!)... com uma vari�vel 'i' que varia, em passos de '1', de '1' at� 'length(vector' (comprimento do 'vector' de entrada)
for i=1:1:length(vector);
    %se o valor na posi��o 'i' do 'vector' de entrada for maior ou igual a 'min' e menor ou igual a 'max'...
    if ( vector(i)>=min && vector(i)<=max );
        %...coloca esse valor (que est� na posi��o 'i' do 'vector') no array 'Valores'
        Valores=[Valores,vector(i)];
        %...coloca o indice desse valor (relativamente ao 'vector') no array 'Indices'
        Indices=[Indices,i];
        %...escreve para o ecr� (...)
        fprintf('\n::: O valor "%g" encontra-se na posi��o "%d" do vector, e pertence ao intervalo [%g,%g]',vector(i),i,min,max);
    end
end
%'NValores' fica com o numero de elementos encontrados no intervalo, que � o comprimento de 'Valores'
NValores=length(Valores);
%escreve no ecr� (...)
fprintf('\n\n::: Foram encontrados %d valores pertencentes ao intervalo [%g,%g]\n\n',NValores,min,max);
%sair da fun��o
return