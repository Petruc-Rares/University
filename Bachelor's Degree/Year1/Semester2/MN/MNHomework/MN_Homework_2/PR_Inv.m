function B = PR_Inv(A)

    [Q, R] = Householder(A);
    
    n = length(A);
    
    B = zeros(n);
    
    % sistemul de rezolvat se reduce la R*B = Q'*I
    % care este un sistem superior triunghiular
    
    A = R;
    
    C = Q'*eye(n);
    
    % calculam inversa pentru fiecare coloana din B
    for i = 1:n  
            B(:, i) = SST(A, C(:, i)); 
    endfor
   
endfunction