function [x] = Jacobi_sparse(G_values, G_colind, G_rowptr, c, tol)

    n = length(c);
    x = zeros(1, n);
    prev_x = x;
    y = zeros(1, n); % vector ajutator
    
    while 1
    
        y = zeros(1, n);
    
        for i = 1:n
        
                y(1, i) = y(1, i) + G_values(1, G_rowptr(1, i):(G_rowptr(1, i+1) - 1)) *  prev_x(1, G_colind(1, G_rowptr(1, i):(G_rowptr(1, i+1) - 1)))';

        endfor

        x = y + c';
            
        err = norm(x - prev_x);
    
        if (err < tol)
    
            break;
  
        endif
        
        prev_x = x;
    
    endwhile

x = x';


endfunction