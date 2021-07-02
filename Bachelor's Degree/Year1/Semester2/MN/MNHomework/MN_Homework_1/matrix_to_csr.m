function [values, colind, rowptr] = matrix_to_csr(A)

    nr = size(A)(1);
    nc = size(A)(2);
    
    
    values = zeros(0, 0);
    colind = zeros(0, 0);
    rowptr = [1];
    k = 1;
    
    for i = 1:nr 
   
       for j = 1:nc
       
           if (A(i, j) != 0)
           
              values = [values, A(i, j)];
              colind = [colind, j];
              k++;
           
           endif
       
       endfor
       
       rowptr = [rowptr, k];
   
    endfor
  

endfunction