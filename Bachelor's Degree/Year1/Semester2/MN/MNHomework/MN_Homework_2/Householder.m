function [Q, R] = Householder(A)
    nl = size(A)(1);
    nc = size(A)(2);
    Q = eye(nl, nl);

    nb = nrPivoti(nl, nc);

    for j = 1:nb
    
        v = zeros(nl, 1);
        H = eye(nl, nl);
        
        simbol = -sign(A(j, j))*norm(A(j:nl,j));
        v(j, 1) = A(j, j) + simbol;
        
        v([j+1:nl], 1) = A([j+1:nl], j);
        
        
        chestie = 2 * v * v' /(v'*v);
        H = H - chestie;

        Q = Q * H';
        A = construieste(A, H, j);
        
    endfor
    
    R = A;
    
endfunction