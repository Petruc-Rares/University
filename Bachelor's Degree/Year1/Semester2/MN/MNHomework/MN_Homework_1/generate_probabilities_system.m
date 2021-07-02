function [A, b] = generate_probabilities_system(rows)
    auxrows = rows;
    s = 1;
    k = 2;
    auxMatrix = [1];
    auxrows--;
    while (auxrows != 0)
        zeroblock = zeros(k-1, 1);
        auxMatrix = [auxMatrix, zeroblock];
        %auxs ne ajuta la lastValue
        auxs = s;
        %s reprezinta numarul total de elemente
        s = s + k;
        %nr = auxMatrix(k-1, 1);
        array = [(auxs + 1):(auxs + (s - auxs))];
        auxMatrix = [auxMatrix; array];
        k = k + 1;
        auxrows--;
    endwhile
    A = zeros(s, s);
    %stabilim valorile default la toate matricile cu rows > 2
    A(1, 1) = 4;
    A(2, 1) = A(1, 2) = -1;
    A(3, 1) = A(1, 3) = -1;
    A(2, 2) = 5;
    A(3, 2) = A(2, 3) = -1;
    A(4, 2) = A(2, 4) = -1;
    A(5, 2) = A(2, 5) = -1;
    A(3, 3) = 5;
    A(5, 3) = A(3, 5) = -1;
    A(6, 3) = A(3, 6) = -1;
    % parcurgem diagrama pe linii pana cand ajungem la ultima linie
    % incepand cu randul al 3 lea
    i = 3;
    while (i != rows)
        %caz special: primul element al liniei
        % are 5 legaturi
        A(auxMatrix(i, 1), auxMatrix(i, 1)) = 5;
        A(auxMatrix(i, 2), auxMatrix(i, 1)) = A(auxMatrix(i, 1), auxMatrix(i, 2)) =  -1;
        A(auxMatrix(i+1, 1), auxMatrix(i, 1)) = A(auxMatrix(i, 1), auxMatrix(i + 1, 1)) = -1;
        A(auxMatrix(i+1, 2), auxMatrix(i, 1)) = A(auxMatrix(i, 1), auxMatrix(i + 1, 2)) =  -1;
        %caz default: elemente cuprinse intre primul si ultimul element al liniei;
        %avem 6 legaturi
        j = 2;
        while (auxMatrix(i, j+1) != 0)
            A(auxMatrix(i, j), auxMatrix(i, j)) = 6;
            A(auxMatrix(i, j+1), auxMatrix(i, j)) = A(auxMatrix(i, j), auxMatrix(i, j+1)) =  -1;
            A(auxMatrix(i+1, j), auxMatrix(i, j)) = A(auxMatrix(i, j), auxMatrix(i+1, j)) =  -1;
            A( auxMatrix(i+1, j+1), auxMatrix(i, j)) = A(auxMatrix(i, j), auxMatrix(i+1, j+1)) =  -1;
            j++;
        endwhile
        % caz special: ultimul element al liniei
        % are 5 legaturi
        A(auxMatrix(i, j), auxMatrix(i, j)) = 5;
        A(auxMatrix(i+1, j), auxMatrix(i, j)) = A(auxMatrix(i, j), auxMatrix(i+1, j)) = -1;
        A(auxMatrix(i+1, j+1), auxMatrix(i, j)) = A(auxMatrix(i, j), auxMatrix(i+1, j+1)) = -1;
        i++;
    endwhile
    %am ajuns la ultimul rand al diagramei
    %coltul stanga are 4 legaturi
    A(auxMatrix(i, 1), auxMatrix(i, 1)) = 4;
    A(auxMatrix(i, 2), auxMatrix(i, 1)) = A(auxMatrix(i, 1), auxMatrix(i, 2)) = -1;
    % cazul default: 5 legaturi
    j = 2;
    while (j != rows)
        A(auxMatrix(i, j), auxMatrix(i, j)) = 5;
        A(auxMatrix(i, j+1), auxMatrix(i, j)) = A(auxMatrix(i, j), auxMatrix(i, j + 1)) =  -1;
        j++;
    endwhile
    %am ajuns la ultimul element al diagramei
    % acesta are 4 legaturi
    A(auxMatrix(i, j), auxMatrix(i, j)) = 4;
    b = zeros(s, 1);
    b((s-rows+1):s,1) = 1;
endfunction