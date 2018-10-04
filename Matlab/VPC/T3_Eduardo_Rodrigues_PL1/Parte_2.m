%% Parte 2 - Detectar rectas e circunferencias

Image = imread('lines_circles_2.jpg');

ImgSize = size(Image);
ImgDim = size(ImgSize);

% Converter a gray
if ImgDim(2) == 3
    gray = rgb2gray(Image);
else
    gray = Image;
end

% Binarizar
BW = edge(gray,'canny');

line_Neighbourhood = 5;
line_Threshold = 350;
radius = 10:40;
circle_Neighbourhood = 10;
circle_Threshold = 30;

%% Detectar Rectas
[ m, b, ~, rhoMax, thetaMax, A_lines ] = Line_Hough(gray, line_Neighbourhood, line_Threshold);

%% Detectar Circulos
[detected_centers,detected_radius,A_circles,~] = Circle_Hough(BW, radius, circle_Neighbourhood, circle_Threshold);

%% Representação de rectas e circulos detectados

% representação da matriz de acumulação e máximos locais para rectas
figure();
subplot(4,5, [1 6 11 16]);
imshow(A_lines, []); xlabel('theta'), ylabel('rho'); hold on;
plot(thetaMax, rhoMax, '*r');

% representação de rectas detectadas
subplot(4,5, [2:5 7:10 12:15 17:20]);
imshow(gray); hold on;
x = [1;ImgSize(2)];
y = m.*x+b;
plot(x, y, 'g'); xlim([1 ImgSize(2)]); ylim([1 ImgSize(1)]);

% representação da matriz de acumulação e máximos locais para circulos
figure();
subplot(1,2,1);
imshow(A_circles(:,:,1), []); xlabel('a'), ylabel('b'); hold on;
plot(detected_centers(:,1),detected_centers(:,2), '*r', 'LineWidth', 2);

% representação de circulos detectados
subplot(1,2,2);
imshow(gray); hold on;
viscircles(detected_centers, detected_radius, 'color', 'green');

%% Detectar rectas - Funções matlab

% Nova imagem para comparação de resultados
Image2 = imread('chess_1.png');

Img2Size = size(Image2);
Img2Dim = size(Img2Size);

% Converter a gray
if Img2Dim(2) == 3
    gray2 = rgb2gray(Image2);
else
    gray2 = Image2;
end

% binarização com filtro canny
BW = edge(gray2,'canny');

% [H,Theta,Rho] = hough(BW);
[H,Theta,Rho] = hough(BW);
% P = houghpeaks(H, 100, 'NHoodSize', [5 5]);
Peaks = houghpeaks(H, 50, 'NHoodSize', [5 5]);
% lines = houghlines(BW,T,R,P);
lines = houghlines(BW,Theta,Rho,Peaks);

% representação da matriz de acumulação e máximos locais
figure();
subplot(4,5, [1 6 11 16]);
imshow(H, [],'XData',Theta,'YData',Rho); xlabel('theta'), ylabel('rho'); hold on;
plot(Theta(Peaks(:,2)),Rho(Peaks(:,1)),'s','color','white');

% representação de rectas detectadas
subplot(4,5, [2:5 7:10 12:15 17:20]);
imshow(gray2); hold on;
for k = 1:length(lines)
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

    % plot beginnings and ends of lines
    plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
    plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');
end