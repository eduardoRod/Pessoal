%% Parte 1

%% 1) Ler imagem e converter para niveis de cinzento

% Ler imagem
Image = imread('cameraman.tif');
% Converter em grayScale

ImgSize = size(Image);
ImgDim = size(ImgSize);

if ImgDim(2) == 3
    GrayImg = rgb2gray(Image);
else
    GrayImg = Image;
end

%% 2) Adicionar diferentes tipos de ruido

% Aplicar ruido gaussiano
% Media zero, variancia 0.1, 0.5, 0.8
ImgGauss0_1Noise = imnoise(GrayImg, 'gaussian', 0, 0.1);
ImgGauss0_5Noise = imnoise(GrayImg, 'gaussian', 0, 0.5);
ImgGauss0_8Noise = imnoise(GrayImg, 'gaussian', 0, 0.8);

% Aplicar ruido salt & pepper
% Densidade 0.5
ImgSaltPepperNoise = imnoise(GrayImg, 'salt & pepper', 0.5);

% Aplicar ruido speckle
% Variancia 0.8
ImgSpeckleNoise = imnoise(GrayImg, 'speckle', 0.8);

%% 3) Aplicar a cada imagem um filtro gaussiano e de mediana


% criar mascara de convolucao para gaussiano
GaussMask = fspecial('gaussian');

% Filtrar imagem com filtro gaussiano
ImgGauss0_1NoiseGaussFilter = imfilter(ImgGauss0_1Noise, GaussMask);
ImgGauss0_5NoiseGaussFilter = imfilter(ImgGauss0_5Noise, GaussMask);
ImgGauss0_8NoiseGaussFilter = imfilter(ImgGauss0_8Noise, GaussMask);
ImgSaltPepperNoiseGaussFilter = imfilter(ImgSaltPepperNoise, GaussMask);
ImgSpeckleNoiseGaussFilter = imfilter(ImgSpeckleNoise, GaussMask);

% Filtrar imagem com filtro mediana
% FilteredImg = medfilt2(Imagem);
ImgGauss0_1NoiseMedFilter = medfilt2(ImgGauss0_1Noise);
ImgGauss0_5NoiseMedFilter = medfilt2(ImgGauss0_5Noise);
ImgGauss0_8NoiseMedFilter = medfilt2(ImgGauss0_8Noise);
ImgSaltPepperNoiseMedFilter = medfilt2(ImgSaltPepperNoise);
ImgSpeckleNoiseMedFilter = medfilt2(ImgSpeckleNoise);

%% 3a) Comparar o desempenho dos varios filtros 

xi = [0 ImgSize(1)];
%yi = [ImgSize(1)/2 ImgSize(1)/2];
yi = [50 50];

% Extrair valores ao longo da recta escolhida para imagens com ruido
%ImgGauss0_1NoiseLine = improfile(ImgGauss0_1Noise, xi, yi);
%ImgGauss0_5NoiseLine = improfile(ImgGauss0_5Noise, xi, yi);
%ImgGauss0_8NoiseLine = improfile(ImgGauss0_8Noise, xi, yi);
ImgSaltPepperNoiseLine = improfile(ImgSaltPepperNoise, xi, yi);
%ImgSpeckleNoiseLine = improfile(ImgSpeckleNoise, xi, yi);

% Extrair valores ao longo da recta escolhida para imagens filtradas com
% gaussiana
%ImgGauss0_1NoiseGaussFilterLine = improfile(ImgGauss0_1NoiseGaussFilter, xi, yi);
%ImgGauss0_5NoiseGaussFilterLine = improfile(ImgGauss0_5NoiseGaussFilter, xi, yi);
%ImgGauss0_8NoiseGaussFilterLine = improfile(ImgGauss0_8NoiseGaussFilter, xi, yi);
ImgSaltPepperNoiseGaussFilterLine = improfile(ImgSaltPepperNoiseGaussFilter, xi, yi);
%ImgSpeckleNoiseGaussFilterLine = improfile(ImgSpeckleNoiseGaussFilter, xi, yi);

% Extrair valores ao longo da recta escolhida para imagens filtradas com
% mediana
%ImgGauss0_1NoiseMedFilterLine = improfile(ImgGauss0_1NoiseMedFilter, xi, yi);
%ImgGauss0_5NoiseMedFilterLine = improfile(ImgGauss0_5NoiseMedFilter, xi, yi);
%ImgGauss0_8NoiseMedFilterLine = improfile(ImgGauss0_8NoiseMedFilter, xi, yi);
ImgSaltPepperNoiseMedFilterLine = improfile(ImgSaltPepperNoiseMedFilter, xi, yi);
%ImgSpeckleNoiseMedFilterLine = improfile(ImgSpeckleNoiseMedFilter, xi, yi);

% Representar valores para cada ruido e efeito dos filtros
% figure();
% hold on;
% plot(ImgGauss0_1NoiseLine);
% plot(ImgGauss0_1NoiseGaussFilterLine);
% plot(ImgGauss0_1NoiseMedFilterLine);
% legend('0.1 Gauss Noise', 'Gauss Filter', 'Median Filter');
% 
% figure();
% hold on;
% plot(ImgGauss0_5NoiseLine);
% plot(ImgGauss0_5NoiseGaussFilterLine);
% plot(ImgGauss0_5NoiseMedFilterLine);
% legend('0.5 Gauss Noise', 'Gauss Filter', 'Median Filter');
% 
% figure();
% hold on;
% plot(ImgGauss0_8NoiseLine);
% plot(ImgGauss0_8NoiseGaussFilterLine);
% plot(ImgGauss0_8NoiseMedFilterLine);
% legend('0.8 Gauss Noise', 'Gauss Filter', 'Median Filter');

figure();
hold on;
plot(ImgSaltPepperNoiseLine);
plot(ImgSaltPepperNoiseGaussFilterLine);
plot(ImgSaltPepperNoiseMedFilterLine);
legend('Salt & Pepper Noise', 'Gauss Filter', 'Median Filter');

% figure();
% hold on;
% plot(ImgSpeckleNoiseLine);
% plot(ImgSpeckleNoiseGaussFilterLine);
% plot(ImgSpeckleNoiseMedFilterLine);
% legend('Speckle Noise', 'Gauss Filter', 'Median Filter');

% Imagens com ruido
figure();
subplot(3, 5, 1);
imshow(ImgGauss0_1Noise);
title('0.1 Gauss Noise');
subplot(3, 5, 2);
imshow(ImgGauss0_5Noise);
title('0.5 Gauss Noise');
subplot(3, 5, 3);
imshow(ImgGauss0_8Noise);
title('0.8 Gauss Noise');
subplot(3, 5, 4);
imshow(ImgSaltPepperNoise);
title('Salt&Pepper Noise');
subplot(3, 5, 5);
imshow(ImgSpeckleNoise);
title('Speckle Noise');

% Imagens com filtro Gaussiano
subplot(3, 5, 6);
imshow(ImgGauss0_1NoiseGaussFilter);
subplot(3, 5, 7);
imshow(ImgGauss0_5NoiseGaussFilter);
subplot(3, 5, 8);
imshow(ImgGauss0_8NoiseGaussFilter);
subplot(3, 5, 9);
imshow(ImgSaltPepperNoiseGaussFilter);
subplot(3, 5, 10);
imshow(ImgSpeckleNoiseGaussFilter);

% Imagens com filtro Mediana
subplot(3, 5, 11);
imshow(ImgGauss0_1NoiseMedFilter);
subplot(3, 5, 12);
imshow(ImgGauss0_5NoiseMedFilter);
subplot(3, 5, 13);
imshow(ImgGauss0_8NoiseMedFilter);
subplot(3, 5, 14);
imshow(ImgSaltPepperNoiseMedFilter);
subplot(3, 5, 15);
imshow(ImgSpeckleNoiseMedFilter);

%% 3b) Determinar influencia do desvio padrao no filtro gaussiano

GaussMask1 = fspecial('gaussian', 3, 0.05);
GaussMask2 = fspecial('gaussian', 3, 0.1);
GaussMask3 = fspecial('gaussian', 3, 0.5);
GaussMask4 = fspecial('gaussian', 3, 0.8);
GaussMask5 = fspecial('gaussian', 3, 0.9);
GaussMask6 = fspecial('gaussian', 3, 1);

GaussMask_0_05_Filtered = imfilter(ImgGauss0_1Noise, GaussMask1);
GaussMask_0_1_Filtered = imfilter(ImgGauss0_1Noise, GaussMask2);
GaussMask_0_5_Filtered = imfilter(ImgGauss0_1Noise, GaussMask3);
GaussMask_0_8_Filtered = imfilter(ImgGauss0_1Noise, GaussMask4);
GaussMask_0_9_Filtered = imfilter(ImgGauss0_1Noise, GaussMask5);
GaussMask_1_Filtered = imfilter(ImgGauss0_1Noise, GaussMask6);

figure();
subplot(2, 3, 1);
imshow(GaussMask_0_05_Filtered);
title('0.05 Gauss Filter');
subplot(2, 3, 2);
imshow(GaussMask_0_1_Filtered);
title('0.1 Gauss Filter');
subplot(2, 3, 3);
imshow(GaussMask_0_5_Filtered);
title('0.5 Gauss Filter');
subplot(2, 3, 4);
imshow(GaussMask_0_8_Filtered);
title('0.8 Gauss Filter');
subplot(2, 3, 5);
imshow(GaussMask_0_9_Filtered);
title('0.9 Gauss Filter');
subplot(2, 3, 6);
imshow(GaussMask_1_Filtered);
title('1.0 Gauss Filter');

%% 4) Implementar filtro de mediana e comparar o seu desempenho com medfilt2

tic;
MatlabMedFilter = medfilt2(ImgGauss0_1Noise);
MatlabTime = toc

tic;
MyMedFilter = MedFilter(ImgGauss0_1Noise, 3);
MyTime = toc

DiferentPixels = sum(sum(MatlabMedFilter~=MyMedFilter))
ImgSize = size(ImgGauss0_1Noise);
Diference = (DiferentPixels/(ImgSize(1)*ImgSize(2)))*100
TimeDiferente = MyTime - MatlabTime

clear;