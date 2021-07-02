function[R] = getAlgebraicPR(N, M, d)

    R = zeros(N, 1);
    R(:) = 1/N;
    
    invers = PR_Inv(eye(N) - d*M);
    
    R = invers*(1-d)/N*ones(N, 1);

endfunction