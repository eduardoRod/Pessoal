function treated = MedFilter(Image, window)

[rows, columns] = size(Image);
treated = zeros(rows, columns, 'uint8');

wSize = window^2;
aux = zeros(wSize, 1);
neighbour = floor(window/2);

for i = 1:rows
    for j = 1:columns
        if (i == 1) || (i == rows) || (j == 1) || (j == columns)
            treated(i, j) = 0;
        else
            position = 1;
            
            for rowSize = -neighbour:neighbour
                for columnSize = -neighbour:neighbour
                    aux(position) = Image(i+rowSize, j+columnSize);
                    position = position + 1;
                end
            end
            
            treated(i, j) = median(aux);
        end
    end
end