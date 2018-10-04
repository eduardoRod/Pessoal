Image = imread('imagens/quadrados.png');
GrayQuadrados = double(rgb2gray(Image));
Image = imread('imagens/LinhasHorizontais.png');
GrayHorizontais = double(rgb2gray(Image));
Image = imread('imagens/LinhasVerticais.png');
GrayVerticais = double(rgb2gray(Image));

[~,~,v1,h1] = edge(GrayQuadrados,'sobel');
[~,~,v2,h2] = edge(GrayHorizontais,'sobel');
[~,~,v3,h3] = edge(GrayVerticais,'sobel');

angleVar = -180:45:225;
anglesQuadrados = atan2d(h1, v1);
anglesHorizontais = atan2d(h2, v2);
anglesVerticais = atan2d(h3, v3);

figure();
histogram(anglesQuadrados(anglesQuadrados ~= 0), angleVar);
xlim([-180, 225]);
set(gca, 'XTick', angleVar);
figure();
histogram(anglesHorizontais(anglesHorizontais ~= 0), angleVar);
xlim([-180, 225]);
set(gca, 'XTick', angleVar);
figure();
histogram(anglesVerticais(anglesVerticais ~= 0), angleVar);
xlim([-180, 225]);
set(gca, 'XTick', angleVar);