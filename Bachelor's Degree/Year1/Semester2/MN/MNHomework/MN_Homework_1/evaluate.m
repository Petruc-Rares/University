function [percentage] = evaluate(path_to_testset, w, histogram, count_bins)
    
    in_cats = getCatImgs(path_to_testset);
    
    no_in_cats = size(in_cats)(1);
    
    in_no_cats = getNoCatImgs(path_to_testset);
    
    no_in_no_cats = size(in_no_cats)(1);
    
    increment = 0;
    
    matrixtoscale = [];
    
    if (strcmp(histogram, "RGB") == 1)
    
        for i = 1:no_in_cats

            test_x = rgbHistogram(in_cats(i, :), count_bins);
            matrixtoscale = [matrixtoscale; test_x];
            %{
            if (test_x * w >= 0) 
                increment++;
            endif
            %}

        endfor

        for i = 1:no_in_no_cats

            test_x = rgbHistogram(in_no_cats(i, :), count_bins);
            matrixtoscale = [matrixtoscale; test_x];
            %{
            if (test_x * w < 0)
                increment++;
            endif
            %}

        endfor

        else 
            for i = 1:no_in_cats

            test_x = hsvHistogram(in_cats(i, :), count_bins);
            matrixtoscale = [matrixtoscale; test_x];

        endfor

        for i = 1:no_in_no_cats

            test_x = hsvHistogram(in_no_cats(i, :), count_bins);

            matrixtoscale = [matrixtoscale; test_x];

        endfor
    
    endif
 
    matrixtoscale = matrixtoscale - mean(matrixtoscale);
    
    matrixtoscale = matrixtoscale./std(matrixtoscale);
    
    b = zeros(size(matrixtoscale)(1), 1);
    b(1:size(matrixtoscale)(1)) = 1;
    matrixtoscale = [matrixtoscale, b];
    
    for i = 1:no_in_cats
    
        if(matrixtoscale(i, :) * w >=0) 
        
            increment++;
        
        endif

    endfor
    
    for i = no_in_cats+1:no_in_cats+no_in_no_cats
    
        if(matrixtoscale(i, :) * w < 0) 
        
            increment++;
        
        endif

    endfor

    percentage = increment/(no_in_cats + no_in_no_cats);

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

