function [LSorted, eigVectorsSorted] = Corner_Detect(Image, window, MinEigValue)
    
    neighbour = floor(window/2);
    MAX_DIST = sqrt((neighbour^2)*2);
    
    ImgSize = size(Image);
    ImgDim = size(ImgSize);
    
    % Convert to grayscale if Image is rgb
    if ImgDim(2) == 3
        gray = rgb2gray(Image);
    else
        gray = Image;
    end
    
    DetectionArea = (ImgSize(1)-neighbour)*(ImgSize(2)-neighbour);
    L = zeros(DetectionArea,3);
    eigVectors = zeros(DetectionArea,2);
    Q = zeros(window, window,2);
    
    % X, Y gradient with Sober mask
    [Gx, Gy] = imgradientxy(gray, 'sobel');
    
    detected = 1;
    for i = (1+neighbour):ImgSize(1)-neighbour
        for j = (1+neighbour):ImgSize(2)-neighbour
            Q(:,:,1) = Gx(i-neighbour:i+neighbour,j-neighbour:j+neighbour);
            Q(:,:,2) = Gy(i-neighbour:i+neighbour,j-neighbour:j+neighbour);
            
            somatorio(1) = sum(sum(Q(:,:,1).^2));
            somatorio(2) = sum(sum(Q(:,:,1).*Q(:,:,2)));
            somatorio(3) = sum(sum(Q(:,:,2).^2));
            
            C = [somatorio(1) somatorio(2);
                 somatorio(2) somatorio(3)];
            
            [eigVectorsAux,eigDiag] = eig(C);
            [lambda,ind] = min(diag(eigDiag));
            
            % keeps all eig values detected
            L(detected,:) = [i j lambda];
            % eigVector associated with eigValue lambda
            eigVectors(detected,:) = eigVectorsAux(:,ind);
            detected = detected + 1;
            
        end
    end
    
    % order elements
    [~,idx] = sort(L(:,3),'descend');
    LSorted = L(idx,:);
    eigVectorsSorted = eigVectors(idx,:);
    
    % cut elemnts under MinEigValue
    wantedElements = LSorted(:,3) >= MinEigValue;
    LSorted = LSorted(wantedElements, :);
    eigVectorsSorted = eigVectorsSorted(wantedElements,:);
    
    i = 1;
    while i < size(LSorted,1)
        x = LSorted(i,1);
        y = LSorted(i,2);
        
        j = i + 1;
        while j < size(LSorted,1)
            if pdist2([x y], [LSorted(j,1) LSorted(j,2)]) <= MAX_DIST
                LSorted(j,:) = [];
                eigVectorsSorted(j,:) = [];
            else
                j = j + 1;
            end
        end
        i = i + 1;
    end
end