function [matriz,Valor,Linha,Coluna] = maiorPosicao(matriz);

Linha=[]; Coluna=[]; Valor=[];


[linhas,colunas]=size(matriz);

clc
for (x = 1:linhas )
    for ( y = 1:colunas )
        if( matriz(x,y) == max(max(matriz)))
        Valor = matriz(x,y);
        Linha = x;
        Coluna = y;
        end
    end
end
return
