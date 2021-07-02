function [X, y] = preprocess(path_to_dataset, histogram, count_bins)

    in_cats = getCatImgs(path_to_dataset);
    
    no_in_cats = size(in_cats)(1);
    
    in_no_cats = getNoCatImgs(path_to_dataset);
    
    no_in_no_cats = size(in_no_cats)(1);
    
    height = size(in_cats)(1) + size(in_no_cats)(1);
    
    X_1 = zeros(no_in_cats, count_bins * 3);
    X_2 = zeros(no_in_no_cats, count_bins * 3);
    X = zeros(no_in_cats + no_in_no_cats, count_bins * 3);
    y_1 = zeros(no_in_cats, 1);
    y_2 = zeros(no_in_no_cats, 1);
    y = zeros(no_in_cats + no_in_no_cats, 1);
    
    for i = 1:no_in_cats
    
        X_1(i, :) = hsvHistogram(in_cats(i, :), count_bins);
        y_1(i, :) = 1;
    
    endfor
    
    for i = 1:no_in_no_cats
    
        X_2(i, :) = hsvHistogram(in_no_cats(i, :), count_bins);
        y_2(i, :) = -1;
    
    endfor
    
    X = [X_1; X_2];
    y = [y_1; y_2];

endfunction

function [in_cats] = getCatImgs(path_to_dataset)

    path_to_cat_imgs = strcat(path_to_dataset, 'cats/');
    
    img_files = dir(path_to_cat_imgs);
    
    	% numarul de imagini
	n = length(img_files);
	
	% lungimea numelui unui fisier
	D = 1:10;
    
	imgs(1:n, D) = 0;

	for i = 1:n
		l = 1:length(img_files(i).name);
		imgs(i, l) = img_files(i).name;
	end

	imgs = char(imgs);
    
    imgs = imgs(3:n-1, D);


    in_cats = strcat(path_to_cat_imgs, imgs);

endfunction

function [in_cats] = getNoCatImgs(path_to_dataset)

    path_to_cat_imgs = strcat(path_to_dataset, 'not_cats/');
    
    img_files = dir(path_to_cat_imgs);
    
    	% numarul de imagini
	n = length(img_files);
	
	% lungimea numelui unui fisier
	D = 1:10;
    
	imgs(1:n, D) = 0;

	for i = 1:n
		l = 1:length(img_files(i).name);
		imgs(i, l) = img_files(i).name;
	end

	imgs = char(imgs);
    
    imgs = imgs(3:n-1, D);

    in_cats = strcat(path_to_cat_imgs, imgs);

endfunction

