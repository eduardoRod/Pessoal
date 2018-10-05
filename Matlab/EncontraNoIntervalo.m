function [Valores,Indices,NValores]=EncontraNoIntervalo(vector);

ValoresPrimeiro=[]; IndicesPrimeiro=[];
ValoresSegundo=[]; IndicesSegundo=[];
ValoresTerceiro=[]; IndicesTerceiro=[];
ValoresQuarto=[]; IndicesQuarto=[];

clc;
for i=1:1:length(vector);
    if ( vector(i)>=0 && vector(i)<=1 );
        ValoresPrimeiro=[ValoresPrimeiro,vector(i)];
        IndicesPrimeiro=[IndicesPrimeiro,i];
        fprintf('\n::: O valor "%g" encontra-se na posição "%d" do vector, e pertence ao intervalo [0,1]',vector(i),i);
    else if ( vector(i)>1 && vector(i)<=2 );
            ValoresSegundo=[ValoresSegundo,vector(i)];
            IndicesSegundo=[IndicesSegundo,i];
            fprintf('\n::: O valor "%g" encontra-se na posição "%d" do vector, e pertence ao intervalo [1,2[',vector(i),i);
        else if ( vector(i)>2 && vector(i)<=3 );
                ValoresSegundo=[ValoresSegundo,vector(i)];
                IndicesSegundo=[IndicesSegundo,i];
                fprintf('\n::: O valor "%g" encontra-se na posição "%d" do vector, e pertence ao intervalo [2,3[',vector(i),i);
            else if ( vector(i)>3 && vector(i)<=4 );
                    ValoresSegundo=[ValoresSegundo,vector(i)];
                    IndicesSegundo=[IndicesSegundo,i];
                    fprintf('\n::: O valor "%g" encontra-se na posição "%d" do vector, e pertence ao intervalo [3,4[',vector(i),i);
                end
            end
        end
    end
end

NValoresPrimeiro=length(ValoresPrimeiro);
NValoresSegundo=length(ValoresSegundo);
NValoresTerceiro=length(ValoresTerceiro);
NValoresQuarto=length(ValoresQuarto);
fprintf('\n\n::: Foram encontrados %d valores pertencentes ao intervalo [0,1]\n',NValoresPrimeiro);
fprintf('::: Foram encontrados %d valores pertencentes ao intervalo [1,2[\n',NValoresSegundo);
fprintf('::: Foram encontrados %d valores pertencentes ao intervalo [2,3[\n',NValoresTerceiro);
fprintf('::: Foram encontrados %d valores pertencentes ao intervalo [3,4[\n',NValoresQuarto)

return