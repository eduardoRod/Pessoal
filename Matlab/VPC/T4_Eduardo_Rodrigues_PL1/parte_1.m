%% Parte 1 trabalho laboratorial 4
clear all; close all; clc;

Image = imread('A1.bmp');
PointsFile = 'A1_2.pts';

[Params] = findCameraParams(Image, PointsFile);

figure();
imshow(Image); hold on;
plot(LSorted(:,2),LSorted(:,1), '*r'); hold off;