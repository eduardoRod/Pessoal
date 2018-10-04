%-------------------------------------------------------------------------
%   Ivo Micael Frazão Costeira, 2013141479
%   Ricardo Manuel Teixeira Pereira, 2013153797
%   Visão Por Computador - Trabalho Prático nº6
%   Estimação da Matriz Essencial, da Matriz Fundamental e Reconstrução 3D
%-------------------------------------------------------------------------
clear; clc; close all;
mkdir('Imagens'); delete Imagens/*.*

%% Leitura das imagens
left1 = imread('left01.jpg');
left2 = imread('left02.jpg');
left3 = imread('left03.jpg');
right1 = imread('right01.jpg');
right2 = imread('right02.jpg');
right3 = imread('right03.jpg');

%% detectCheckerboardPoints
left1_checker = detectCheckerboardPoints(left1);
left2_checker = detectCheckerboardPoints(left2);
left3_checker = detectCheckerboardPoints(left3);
right1_checker = detectCheckerboardPoints(right1);
right2_checker = detectCheckerboardPoints(right2);
right3_checker = detectCheckerboardPoints(right3);

%% Visualização
f1 = figure; imshow(left1, []); hold on;
plot(left1_checker(:,1), left1_checker(:,2), 'r+');
f2 = figure; imshow(left2, []); hold on;
plot(left2_checker(:,1), left2_checker(:,2), 'r+');
f3 = figure; imshow(left3, []); hold on;
plot(left3_checker(:,1), left3_checker(:,2), 'r+');
f4 = figure; imshow(right1, []); hold on;
plot(right1_checker(:,1), right1_checker(:,2), 'r+');
f5 = figure; imshow(right2, []); hold on;
plot(right2_checker(:,1), right2_checker(:,2), 'r+');
f6 = figure; imshow(right3, []); hold on;
plot(right3_checker(:,1), right3_checker(:,2), 'r+');

%% Normalização dos dados
% Left
xLeft_checker = [left1_checker(:,1); left2_checker(:,1); left3_checker(:,1)];
yLeft_checker = [left1_checker(:,2); left2_checker(:,2); left3_checker(:,2)];

nLeft = size(xLeft_checker, 1);
xLeft_med = sum(xLeft_checker)/nLeft;
yLeft_med = sum(yLeft_checker)/nLeft;
dLeft_med = sum(hypot(xLeft_checker-xLeft_med,...
    yLeft_checker-yLeft_med))./(nLeft.*sqrt(2));

T_left = [1 0 -xLeft_med; 0 1 -yLeft_med; 0 0 dLeft_med];
xLeft_normalized = (xLeft_checker-xLeft_med)/dLeft_med;
yLeft_normalized = (yLeft_checker-yLeft_med)/dLeft_med;

% Right
xRight_checker = [right1_checker(:,1); right2_checker(:,1);...
    right3_checker(:,1)];
yRight_checker = [right1_checker(:,2); right2_checker(:,2);...
    right3_checker(:,2)];

nRight = size(xRight_checker, 1);
xRight_med = sum(xRight_checker)/nRight;
yRight_med = sum(yRight_checker)/nRight;
dRight_med = sum(hypot(xRight_checker-xRight_med,...
    yRight_checker-yRight_med))/(nRight*sqrt(2));

T_right = [1 0 -xRight_med; 0 1 -yRight_med; 0 0 dRight_med];
xRight_normalized = (xRight_checker-xRight_med)/dRight_med;
yRight_normalized = (yRight_checker-yRight_med)/dRight_med;

% Junção dos pontos normalizados
xne = xLeft_normalized;
yne = yLeft_normalized;
xnd = xRight_normalized;
ynd = yRight_normalized;

Left_points_normalized = [xLeft_normalized, yLeft_normalized,...
    ones(size(xne, 1),1)];
Right_points_normalized = [xRight_normalized, yRight_normalized, ...
    ones(size(xnd, 1),1)];

lpn = Left_points_normalized;
rpn = Right_points_normalized;

%% Construção da Matriz A
A = [xnd.*xne, xnd.*yne, xnd, ynd.*xne, ynd.*yne, ynd, xne, yne,...
    ones(size(xne,1),1)];

%% Decomposição em valores singulares
[U,S,V] = svd(A);

%% Cálculo da Matriz F
F = [V(1,9) V(2,9) V(3,9);
    V(4,9) V(5,9) V(6,9);
    V(7,9) V(8,9) V(9,9)];

[Uf, Sf, Vf] = svd(F);
Sf(3,3) = 0;
F = Uf*Sf*transpose(Vf);

% Desnormalização da matriz F
F = transpose(T_right)*F*T_left;
F = F./norm(F, 'fro');

%% Estimação dos epipolos
[Uf, Sf, Vf] = svd(F);
% Epipólo esquerdo
Epi_left = Vf(:,3);
Epi_left = Epi_left/Epi_left(3);
%Epipólo direito
Epi_right = Uf(:,3);
Epi_right = Epi_right/Epi_right(3);

%% Representação dos epipolos na imagens
% Imagem 1
% Epipolo direito
f7 = figure; hold on;
plot(Epi_right(1), Epi_right(2), 'r.');
imshow(right1,[]);

% Epipolo esquerdo
f8 = figure; hold on;
plot(Epi_left(1), Epi_left(2), 'r.');
imshow(left1,[]);

% Imagem 2
% Epipolo direito
f9 = figure; hold on;
plot(Epi_right(1), Epi_right(2), 'r.');
imshow(right2,[]);

% Epipolo esquerdo
f10 = figure; hold on;
plot(Epi_left(1), Epi_left(2), 'r.');
imshow(left2,[]);

% Imagem 3
% Epipolo direito
f11 = figure; hold on;
plot(Epi_right(1), Epi_right(2), 'r.');
imshow(right3,[]);

% Epipolo esquerdo
f12 = figure; hold on;
plot(Epi_left(1), Epi_left(2), 'r.');
imshow(left3,[]);

%% Representação nas imagens das retas epipolares -> Exercício 3
%% Imagem 1
% Direita
f13 = figure; hold on;
n = size(right1_checker,1); % Número de pontos existentes
plot(Epi_right(1), Epi_right(2),'r.'); hold on;
imshow(right1,[]); hold on;
for i = 1:n
    plot([Epi_right(1) right1_checker(i,1)],...
        [Epi_right(2) right1_checker(i,2)]);
    hold on;
    %plot(right1_checker(i,1), right1_checker(i,2),'r+');
end

% Esquerda
f14 = figure; hold on;
n = size(left1_checker,1); % Número de pontos existentes
plot(Epi_left(1), Epi_left(2),'r.'); hold on;
imshow(left1,[]); hold on;
for i = 1:n
    plot([Epi_left(1) left1_checker(i,1)],...
        [Epi_left(2) left1_checker(i,2)]);
    hold on;
end

%% Imagem 2
% Direita
f15 = figure; hold on;
n = size(right2_checker,1); % Número de pontos existentes
plot(Epi_right(1), Epi_right(2),'r.'); hold on;
imshow(right2,[]); hold on;
for i = 1:n
    plot([Epi_right(1) right2_checker(i,1)],...
        [Epi_right(2) right2_checker(i,2)]);
    hold on;
end

% Esquerda
f16 = figure; hold on;
n = size(left2_checker,1); % Número de pontos existentes
plot(Epi_left(1), Epi_left(2),'r.'); hold on;
imshow(left2,[]); hold on;
for i = 1:n
    plot([Epi_left(1) left2_checker(i,1)],...
        [Epi_left(2) left2_checker(i,2)]);
    hold on;
end

%% Imagem 3
% Direita
f17 = figure; hold on;
n = size(right3_checker,1); % Número de pontos existentes
plot(Epi_right(1), Epi_right(2),'r.'); hold on;
imshow(right3,[]); hold on;
for i = 1:n
    plot([Epi_right(1) right3_checker(i,1)],...
        [Epi_right(2) right3_checker(i,2)]);
    hold on;
end

% Esquerda
f18 = figure; hold on;
n = size(left3_checker,1); % Número de pontos existentes
plot(Epi_left(1), Epi_left(2),'r.'); hold on;
imshow(left3,[]); hold on;
for i = 1:n
    plot([Epi_left(1) left3_checker(i,1)],...
        [Epi_left(2) left3_checker(i,2)]);
    hold on;
end

%% Exercício 4
Left_points = [left1_checker; left2_checker; left3_checker];
Right_points = [right1_checker; right2_checker; right3_checker];
[F_Matlab, inliers] = estimateFundamentalMatrix(Left_points,...
    Right_points, 'Method', 'Norm8Point');

%% Imagem 1
% Esquerda
[~, epipole] = isEpipoleInImage(F_Matlab, size(left1));
f19 = figure; plot(epipole(1), epipole(2), 'r+'); hold on;
imshow(left1, []); hold on;
plot(left1_checker(inliers(1:size(left1_checker,1)),1),...
    left1_checker(inliers(1:size(left1_checker,1)),2), 'g+'); hold on;
for i = 1:1:size(left1_checker,1)
    plot([epipole(1) left1_checker(i,1)],...
    [epipole(2) left1_checker(i,2)]); hold on;
end

% Direita
[~, epipole] = isEpipoleInImage(F_Matlab', size(right1));
f20 = figure; plot(epipole(1), epipole(2), 'r+'); hold on;
imshow(right1, []); hold on;
plot(right1_checker(inliers(1:size(right1_checker,1)),1),...
    right1_checker(inliers(1:size(right1_checker,1)),2), 'g+'); hold on;
for i = 1:1:size(right1_checker,1)
    plot([epipole(1) right1_checker(i,1)],...
    [epipole(2) right1_checker(i,2)]); hold on;
end

%% Imagem 2
% Esquerda
[~, epipole] = isEpipoleInImage(F_Matlab, size(left2));
f21 = figure; plot(epipole(1), epipole(2), 'r+'); hold on;
imshow(left2, []); hold on;
n1 = size(left1_checker,1);
n2 = size(left2_checker,1);
plot(left2_checker(inliers(n1+1:n1+n2),1),...
    left2_checker(inliers(n1+1:n1+n2),2), 'g+'); hold on;
for i = 1:1:size(left2_checker,1)
    plot([epipole(1) left2_checker(i,1)],...
    [epipole(2) left2_checker(i,2)]); hold on;
end

% Direita
[~, epipole] = isEpipoleInImage(F_Matlab', size(right2));
f22 = figure; plot(epipole(1), epipole(2), 'r+'); hold on;
imshow(right2, []); hold on;
n1 = size(right1_checker,1);
n2 = size(right2_checker,1);
plot(right2_checker(inliers(n1+1:n1+n2),1),...
    right2_checker(inliers(n1+1:n1+n2),2), 'g+'); hold on;
for i = 1:1:size(right2_checker,1)
    plot([epipole(1) right2_checker(i,1)],...
    [epipole(2) right2_checker(i,2)]); hold on;
end

%% Imagem 3
% Esquerda
[~, epipole] = isEpipoleInImage(F_Matlab, size(left3));
f23 = figure; plot(epipole(1), epipole(2), 'r+'); hold on;
imshow(left3, []); hold on;
n1 = size(left2_checker,1);
n2 = size(left3_checker,1);
plot(left3_checker(inliers(n1+1:n1+n2),1),...
    left3_checker(inliers(n1+1:n1+n2),2), 'g+'); hold on;
for i = 1:1:size(left3_checker,1)
    plot([epipole(1) left3_checker(i,1)],...
    [epipole(2) left3_checker(i,2)]); hold on;
end

% Direita
[~, epipole] = isEpipoleInImage(F_Matlab', size(right3));
f24 = figure; plot(epipole(1), epipole(2), 'r+'); hold on;
imshow(right3, []); hold on;
n1 = size(right2_checker,1);
n2 = size(right3_checker,1);
plot(right3_checker(inliers(n1+1:n1+n2),1),...
    right3_checker(inliers(n1+1:n1+n2),2), 'g+'); hold on;
for i = 1:1:size(right3_checker,1)
    plot([epipole(1) right3_checker(i,1)],...
    [epipole(2) right3_checker(i,2)]); hold on;
end

%% Parte 2
K_left = [533.0031 0 341.586;
    0 533.1526 234.259;
    0 0 1];
K_right = [536.9826 0 326.472;
    0 536.56938 249.3326;
    0 0 1];

%% Exercício 5 -> Matriz Essencial
E = transpose(K_right)*F*K_left;
rank(E)
% Tem rank = 2, está correto.

%% Exercício 6 -> Matriz rotação e translação 
% Decomposição da matriz E em valores singulares
E_Matlab = transpose(K_right)*F_Matlab*K_left;
[Ue,Se,Ve] = svd(E_Matlab);
Se(1,1) = 1;
Se(2,2) = 1;
Se(3,3) = 0;
E = Ue*Se*Ve';

leftPoints = inv(K_left)*lpn';
rightPoints = inv(K_right)*rpn';

W = [0 -1 0; 1 0 0; 0 0 1];
% Considerando, como na aula teórica, o ponto P_esquerdo como referência

R = Ue*W*Ve';
t = -Ue(:,3); % Só se sabe a direção, não se sabe o fator de escala

x = Ue*[cos(pi/2) -sin(pi/2) 0; sin(pi/2) cos(pi/2) 0; 0 0 1]*Se*Ue';
T = [x(3,2); x(1,3); x(2,1)];
T./t

%% Exercício 7 -> Reconstrução 3D
%% Método do matlab (triangulate)
cameraLeftParams = cameraParameters('IntrinsicMatrix', K_left');
cameraRightParams = cameraParameters('IntrinsicMatrix', K_right');
stereoParams = stereoParameters(cameraLeftParams, cameraRightParams, R, t);

worldPoints1 = triangulate(lpn(1:54,1:2), rpn(1:54,1:2), stereoParams);
worldPoints2 = triangulate(lpn(54+1:54+54,1:2), rpn(54+1:54+54,1:2), stereoParams);
worldPoints3 = triangulate(lpn(54+54+1:54+54+72,1:2), rpn(54+54+1:54+54+72,1:2), stereoParams);
f25 = figure;
plot3(worldPoints1(:,1), worldPoints1(:,2), worldPoints1(:,3), 'r*');
hold on;
plot3(worldPoints2(:,1), worldPoints2(:,2), worldPoints2(:,3), 'g*');
hold on;
plot3(worldPoints3(:,1), worldPoints3(:,2), worldPoints3(:,3), 'b*');
hold on;
plotCamera('Location', [0 0 0], 'Orientation', eye(3), 'Opacity', 1,...
    'Size', 0.05); hold on;
plotCamera('Location', t, 'Orientation', R, 'Opacity', 1,...
    'Size', 0.05);
grid on;

f26 = figure;
subplot(2,2,1); imshow(right1,[]); title('Right 1');
subplot(2,2,2); imshow(right2,[]); title('Right 2');
subplot(2,2,3); imshow(right3,[]); title('Right 3');
subplot(2,2,4); title('Reconstrução');
plot3(worldPoints1(:,1), worldPoints1(:,2), worldPoints1(:,3), 'r*'); hold on;
plot3(worldPoints2(:,1), worldPoints2(:,2), worldPoints2(:,3), 'g*'); hold on;
plot3(worldPoints3(:,1), worldPoints3(:,2), worldPoints3(:,3), 'b*');
grid on;
legend('Right 1', 'Right 2', 'Right 3');

%% Prints
print(f1, '-depsc', 'Imagens/1_Left1');
print(f2, '-depsc', 'Imagens/1_Left2');
print(f3, '-depsc', 'Imagens/1_Left3');
print(f4, '-depsc', 'Imagens/1_Right1');
print(f5, '-depsc', 'Imagens/1_Right2');
print(f6, '-depsc', 'Imagens/1_Right3');
print(f7, '-depsc', 'Imagens/2_EpipoleLeft1');
print(f8, '-depsc', 'Imagens/2_EpipoleLeft2');
print(f9, '-depsc', 'Imagens/2_EpipoleLeft3');
print(f10, '-depsc', 'Imagens/2_EpipoleRight1');
print(f11, '-depsc', 'Imagens/2_EpipoleRight2');
print(f12, '-depsc', 'Imagens/2_EpipoleRight3');
print(f13, '-depsc', 'Imagens/3_EpipolarLinesRight1');
print(f14, '-depsc', 'Imagens/3_EpipolarLinesLeft1');
print(f15, '-depsc', 'Imagens/3_EpipolarLinesRight2');
print(f16, '-depsc', 'Imagens/3_EpipolarLinesLeft2');
print(f17, '-depsc', 'Imagens/3_EpipolarLinesRight3');
print(f18, '-depsc', 'Imagens/3_EpipolarLinesLeft3');
print(f19, '-depsc', 'Imagens/4_EpipolarLinesLeft1');
print(f20, '-depsc', 'Imagens/4_EpipolarLinesRight1');
print(f21, '-depsc', 'Imagens/4_EpipolarLinesLeft2');
print(f22, '-depsc', 'Imagens/4_EpipolarLinesRight2');
print(f23, '-depsc', 'Imagens/4_EpipolarLinesLeft3');
print(f24, '-depsc', 'Imagens/4_EpipolarLinesRight3');
print(f25, '-depsc', 'Imagens/7_3DReconstructionWithCamera');
print(f26, '-depsc', 'Imagens/7_3DReconstructionNoCamera');
