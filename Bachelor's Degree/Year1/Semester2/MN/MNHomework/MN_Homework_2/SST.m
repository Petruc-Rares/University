function [x] = SST(A, b)

    n = length(b);
    x = zeros(n,1);

    for i = [n:-1:1]
    
        x(i, 1) = (b(i)-A(i, [1:n])*x)/A(i, i);
        
    endfor
    
endfunction