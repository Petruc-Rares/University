function [G_J, c_J] = Jacobi_factorization(A, b)
    
    N = diag(diag(A));
    P = -(tril(A, -1) + triu(A, 1));
    
    G_J = inv(N) * P;
    c_J = inv(N) * b;
    
endfunction
