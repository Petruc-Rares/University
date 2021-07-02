% aplicarea algoritmului explicat pe pagina de Wikipedia
function[prevR] = getIterativePR(N, M, d, eps)

    one = ones(N, 1);
    R = zeros(N, 1);
    prevR = zeros(N, 1);
    prevR(:) = 1/N;
    
    while 1
    
        R = d*M*prevR + (1-d)/N*one;
    
        if(norm(R - prevR) < eps)
            break;
        endif
        
        prevR = R;
    
    endwhile
    
endfunction