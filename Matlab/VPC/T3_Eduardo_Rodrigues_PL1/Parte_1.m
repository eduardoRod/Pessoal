%% Parte 1 - Detectar cantos

% Ler imagem
Image = imread('imagens/chess_2.png');
gray = rgb2gray(Image);

%% LSorted = Corner_Detect(Image, window, minEigValue)
%LSorted = Corner_Detect(Image,3,2000);
[LSorted, ~] = Corner_Detect(gray,3,100000);

%% 2 - Histograma de valores próprios
figure();
histogram(LSorted(:,3));

figure();
imshow(gray); hold on; axis ij;
plot(LSorted(:,2),LSorted(:,1), '*r');


%% 3 - Comparar vectores próprios com janelas 3x3 e 5x5, fundo branco e cinza
chckBrd = checkerboard(10,5,7);

[LSorted3x3, eigVectorsSorted3x3] = Corner_Detect(chckBrd,3,20);
[LSorted5x5, eigVectorsSorted5x5] = Corner_Detect(chckBrd,5,20);

figure();
subplot(1,2,1);
imshow(chckBrd); hold on; axis image;
plot(LSorted3x3(:,2),LSorted3x3(:,1), '*r');
quiver(LSorted3x3(:,2),LSorted3x3(:,1),eigVectorsSorted3x3(:,1),eigVectorsSorted3x3(:,2),0);
title('Janela 3x3');

subplot(1,2,2);
imshow(chckBrd); hold on; axis image;
plot(LSorted5x5(:,2),LSorted5x5(:,1), '*r');
quiver(LSorted5x5(:,2),LSorted5x5(:,1),eigVectorsSorted5x5(:,1),eigVectorsSorted5x5(:,2),0);
title('Janela 5x5');

%% 4 - Comparar tamanho de máscaras e funções matlab

Time = zeros(6,1);

% Janela 3x3
tic;
[LSorted3x3, ~] = Corner_Detect(gray,3,100000);
Time(1) = toc;

% Janela 5x5
tic;
[LSorted5x5, ~] = Corner_Detect(gray,5,100000);
Time(2) = toc;

%detectCheckerboardPoints
tic;
CheckBoardPoints = detectCheckerboardPoints(gray);
Time(3) = toc;

%detectHarrisFeatures
tic;
HarrisCorners = detectHarrisFeatures(gray);
Time(4) = toc;

%detectMinEigenFeatures
tic;
MinEigCorners = detectMinEigenFeatures(gray);
Time(5) = toc;

%detectFASTFeatures
tic;
FastFeaturesCorners = detectFASTFeatures(gray);
Time(6) = toc;

% Apresentar cantos
figure();

subplot(2,3,1);
imshow(gray); hold on;
plot(LSorted3x3(:,2),LSorted3x3(:,1), '*r');
title('Janela 3x3');

subplot(2,3,2);
imshow(gray); hold on;
plot(LSorted5x5(:,2),LSorted5x5(:,1), '*r');
title('Janela 5x5');

subplot(2,3,3);
imshow(gray); hold on;
plot(CheckBoardPoints(:,1),CheckBoardPoints(:,2), '*r');
title('detectCheckerboardPoints');

subplot(2,3,4);
imshow(gray); hold on;
plot(HarrisCorners.selectStrongest(500));
title('detectHarrisFeatures');

subplot(2,3,5);
imshow(gray); hold on;
plot(MinEigCorners.selectStrongest(200));
title('detectMinEigenFeatures');

subplot(2,3,6);
imshow(gray); hold on;
plot(FastFeaturesCorners.selectStrongest(200));
title('detectFASTFeatures');