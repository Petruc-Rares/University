function[]  = ex3(x, f)

    n = size(x, 1);
    arrayToCalculate = x.*f;
    disp('mv3');
    disp(mean(arrayToCalculate));
    
    disp('sig3 = ');
    disp(sqrt(1/(n-1)*mean(arrayToCalculate)));

end