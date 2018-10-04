function [ m, b, localMax, rhoMaxIndex, thetaMaxIndex, A ] = Line_Hough( Image, Neighbourhood, threshold )
    
    %% [ m, b, localMax, rhoMaxIndex, thetaMaxIndex, A ] = Line_Hough( Image, Neighbourhood, threshold )
    
    ImgSize = size(Image);
    ImgDim = size(ImgSize);

    % Convert to grayscale if Image is rgb
    if ImgDim(2) == 3
        gray = rgb2gray(Image);
    else
        gray = Image;
    end
    
    % Binarization if Image is not binary
    if max(max(gray)) > 1
        gray = round(double(gray)./255);
    end
    
    MAX_RHO = hypot(ImgSize(1),ImgSize(2));
    rho_delta = MAX_RHO/1000;
    
    % rho and theta declaration
    rho = -MAX_RHO:rho_delta:MAX_RHO;
    theta = -pi/2:pi/360:pi/2;
    
    A = zeros(length(rho), length(theta));
    displacement = find(rho == 0);
    
    for i = 1:ImgSize(2)
        for j = 1:ImgSize(1)
            if gray(j,i) == 1
                for thetaIndex = 1:length(theta)
                    XYRho = i*cos(theta(thetaIndex)) + j*sin(theta(thetaIndex));
                    rhoIndex = ceil((XYRho)/rho_delta)+displacement;
                    A(rhoIndex,thetaIndex) = A(rhoIndex,thetaIndex)+1;
                end
            end
        end
    end
    
    % Local Max mask
    mask = ones(Neighbourhood,Neighbourhood);
    center = ceil(Neighbourhood/2);
    mask(center,center) = 0;
    % find A local maxima above threshold
    localMax = ((A > imdilate(A, mask)) & (A > threshold));
    
    [rhoMaxIndex, thetaMaxIndex] = find(localMax);
    rhoMax = rho(rhoMaxIndex);
    thetaMax = theta(thetaMaxIndex);
    m = -1./tan(thetaMax);
    b = rhoMax./sin(thetaMax);
    
end