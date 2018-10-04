function [detected_centers, detected_radius, A, localMax ] = Circle_Hough( Image, radius, Neighbourhood, threshold )

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
        BW = edge(gray2,'canny');
    else
        BW = gray;
    end
    
    theta = (0:0.1:2*pi)';
    ntheta = length(theta);
    nradius = length(radius);
    
    [x, y] = find(BW);
    npoints = length(x);
    % x and y are [xLength X ntheta X nradius] matrices with coordinates of detected
    % points on image
    x = repmat(x, 1, ntheta, nradius);
    y = repmat(y, 1, ntheta, nradius);
    
    % x(y)_radius are [xLength X ntheta X nradius] matrices with coordinates
    % of points of a circle with radius = nradius and theta = ntheta anc
    % centered in (0,0)
    x_radius(1,:,:) = radius.*cos(theta);
    x_radius = repmat(x_radius, npoints, 1, 1);
    y_radius(1,:,:) = radius.*sin(theta);
    y_radius = repmat(y_radius, npoints, 1, 1);
    
    % possibleCentersA(B) are [xLength X ntheta X nradius] matrices with
    % coordinates of possible centers of circles that having radius =
    % nradius and theta = ntheta would have a point in coordinate (x,y)
    possibleCentersA = round(x - x_radius);
    possibleCentersB = round(y - y_radius);
    
    % exclude centers out of image
    possibleCentersA(possibleCentersA <= 0 ... 
        | possibleCentersA >= ImgSize(1)) = NaN;
    possibleCentersB(possibleCentersB <= 0 ... 
        | possibleCentersB >= ImgSize(2)) = NaN;
    
    A = zeros(ImgSize(1), ImgSize(2), nradius);
    
    for i = 1:nradius
        for j = 1:ntheta
            for k = 1:npoints
                a = possibleCentersA(k,j,i);
                b = possibleCentersB(k,j,i);
                
                if ~isnan(a) && ~isnan(b)
                    A(a,b,i) = A(a,b,i) + 1;
                end
            end
        end
    end
    
    % Local Max mask
    mask = ones(Neighbourhood,Neighbourhood,Neighbourhood);
    center = ceil(Neighbourhood/2);
    mask(center,center,center) = 0;
    % find A local maxima above threshold
    localMax = ((A > imdilate(A, mask)) & (A > threshold));
    
    maxIndex = find(localMax);
    [i, j, k] = ind2sub(size(localMax), maxIndex);
    detected_centers = [j i];
    detected_radius = radius(k);
    
end

