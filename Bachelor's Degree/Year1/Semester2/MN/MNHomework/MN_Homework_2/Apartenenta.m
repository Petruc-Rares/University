function y = Apartenenta(x, val1, val2)

    % calculez valorile a si b obtinute
    % prin impunerea conditiilor de continuitate
    a = 1/(val2-val1);
    b = val1/(val1-val2);

    % y ia valorile corespunzatoare functiei u(x)
    % din pdf
    if ((x>=0) && (x < val1)) y = 0;
    elseif ((x >= val1) && (x <= val2)) y = a*x + b;
    elseif (x >= val2)y = 1;
    endif
   
endfunction
	