function[]  = ex2(x, f)

    arrayToCalculate = x.*f;
    n = size(arrayToCalculate, 2);
    disp('mv2 = ');
    disp(mean(x));
    disp('sig2 = ');
        disp(sqrt(1/(n-1)*mean(arrayToCalculate)));

end