function [sol] = rgbHistogram(path_to_image, count_bins)

    A = imread(path_to_image);
    
    binranges = [0:256/count_bins:256];
    
    
    bincountsR = zeros(1, count_bins + 1);
    bincountsG = zeros(1, count_bins + 1);
    bincountsB = zeros(1, count_bins + 1);
    
    
    sol = histc(A(:, :, 1)(:), binranges)'(1:count_bins);
    sol = [sol, histc(A(:, :, 2)(:), binranges)'(1:count_bins)];
    sol = [sol, histc(A(:, :, 3)(:), binranges)'(1:count_bins)];

end

