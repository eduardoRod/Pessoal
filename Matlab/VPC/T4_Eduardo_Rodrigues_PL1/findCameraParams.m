function [ Params ] = findCameraParams( Image, Points )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    
    ImgSize = size(Image);
    ImgDim = size(ImgSize);

    if ImgDim == 3
        gray = rgb2gray(Image);
    else
        gray = Image;
    end

    if(max(max(gray)) > 1)
        BW = gray./255;
    else
        BW = gray;
    end
    
    [LSorted, ~] = Corner_Detect(BW, 5, 30, 1);
    detectedPoints = [LSorted(:,2) LSorted(:,1)];
    filePoints = fileread(Points);
    
    

end

