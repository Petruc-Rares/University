% functie ce genereaza outputul final
function [R1 R2] = PageRank(nume, d, eps)

    R1 = Iterative(nume, d, eps);    
    R2 = Algebraic(nume, d);
    
    [N, val1, val2] = getNval1and2(nume);

    % PR1 devine vectorul R2 sortat descrescator
    % indexes reprezinta numarul fiecarei pagini
    [PR1, indexes] = sort(R2, 'descend');
   
    % in vector tin minte valoarea functiei aplicata
    % indicelui pagerank al fiecarei pagini in parte
    vector = zeros(1:size(R2)(1), 1);
    
    for i = 1:size(R2)(1)
        vector(i, 1) = Apartenenta(PR1(i, 1), val1, val2);
    endfor
   
    % printam in fisierul de output corespunzator cele cerute
    printToFile(nume, N, R1, R2, indexes, vector);

endfunction

