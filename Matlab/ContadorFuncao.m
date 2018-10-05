function [letras,numeros,simbolos] = ContadorFuncao(string)

a = string;
numeros = 0;
letras = 0;
simbolos = 0;

clc;

for i = 1:size(a')
    if (a(i)>='a'+0 && a(i)<='z'+0 || a(i)>='A'+0 && a(i)<='Z'+0)
        letras = letras+1;
    else if (a(i)>='0'+0 && a(i)<= '9'+0)
            numeros = numeros+1;
        else 
            simbolos = simbolos+1;
        end
    end
end

return

