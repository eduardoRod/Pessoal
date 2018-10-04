%% Parte 2

% Ler imagem
Image = imread('imagens/ImgTest.jpg');

% Converter em grayScale
ImgSize = size(Image);
ImgDim = size(ImgSize);

if ImgDim(2) == 3
    GrayImg = double(rgb2gray(Image));
else
    GrayImg = double(Image);
end
GrayImg = GrayImg./255;

%% 5) Determinar pixeis pertencentes a arestas

% Calcular modulo e direccao do gradiente atraves de Sobel
[Gx, Gy] = imgradientxy(GrayImg, 'sobel');
[Gmag, Gdir] = imgradient(Gx, Gy);
subplot(1, 2, 1);
ImgSize = size(GrayImg);
maxX = [];
maxY = [];
maxU = [];
maxV = [];
% representa apenas os valores que sao maximos numa vizinhanca 3x3
for i = 2:ImgSize(1)-1
    for j = 2:ImgSize(2)-1
        if Gmag(i,j) == max(max(Gmag(i-1:i+1, j-1:j+1)))
            maxX = [maxX, j];
            maxY = [maxY, i];
            maxU = [maxU, Gx(i,j)];
            maxV = [maxV, Gy(i,j)];
        end
    end
end
imshow(Gmag), axis image, hold on;
quiver(maxX, maxY, maxU, maxV, 'r');
title('Sobel Gradient magnitude');
subplot(1, 2, 2);
imshow(Gdir),axis image;
title('Sobel Gradient direction'); hold off;

% criar mascara de convolucao para Sobel, Laplaciano, LoG
SobelMask = fspecial('sobel');
LapMask = fspecial('laplacian');
LoGMask = fspecial('log');

% filtrar imagens
ImgSobelMask = imfilter(GrayImg, SobelMask);
ImgLapMask = imfilter(GrayImg, LapMask);
ImgLoGMask = imfilter(GrayImg, LoGMask);
ImgCannyMask = edge(GrayImg, 'canny');

figure();
subplot(2,2,1), imshow(ImgSobelMask);
title('Sobel Contours');
subplot(2,2,2), imshow(ImgLapMask);
title('Laplacian Contours');
subplot(2,2,3), imshow(ImgLoGMask);
title('LoG Contours');
subplot(2,2,4), imshow(ImgCannyMask);
title('Canny Contours');

%% 6) Obter contornos e determinar a sua orientacao

Image = imread('imagens/quadrados.png');

% Converter em grayScale
ImgSize = size(Image);
ImgDim = size(ImgSize);

if ImgDim(2) == 3
    GrayImg = double(rgb2gray(Image));
else
    GrayImg = double(Image);
end

% Contornos com funcao edge
ImgCannyMask = edge(GrayImg, 'canny');
ImgLoGMask = edge(GrayImg, 'log');
ImgZeroCrossMask = edge(GrayImg,'zerocross');
% Contornos e orientacao para Sobel
[ImgSobelMask,~,v,h] = edge(GrayImg,'sobel');

figure();
subplot(2,2,1), imshow(ImgSobelMask);
title('Sobel Contours');
subplot(2,2,2), imshow(ImgZeroCrossMask);
title('Zero Cross Contours');
subplot(2,2,3), imshow(ImgLoGMask);
title('LoG Contours');
subplot(2,2,4), imshow(ImgCannyMask);
title('Canny Contours');

angleVar = -180:45:225;
grayVar = 0:32:256;
GrayAngleImg = zeros(ImgSize(1),ImgSize(2));

% Histograma  de orientacao
figure();
angles = atan2d(h, v);
histogram(angles(angles ~= 0), angleVar);
xlim([-180, 225]);
set(gca, 'XTick', angleVar);

% display vectores de orientacao das arestas
figure();
quiver(h, v), axis ij;

% conversao de angulos para valores de cinza
angles(angles == 0) = 20;
[lia1,locb1] = ismember(angles,angleVar);
GrayAngleImg(lia1) = grayVar(locb1(lia1));
figure(), imshow(GrayAngleImg);

clear;