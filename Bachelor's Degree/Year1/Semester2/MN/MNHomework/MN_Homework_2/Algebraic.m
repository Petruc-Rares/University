% functie ce calculeaza vectorul
% de pagerank R in mod algebric
function R = Algebraic(nume, d)
    % obtinem numarul de pagini si
    % matricea M
    [N, M] = getNM(nume);
    
    % numele fisierului de output
    output = numeFisOutput(nume);
    
    % initializez vectorul de PageRank corespunzator
    R = zeros(N, 1);
    R(:) = 1/N;
    
    % obtinem vectorul PageRank
    inversa = PR_Inv(eye(N)-d*M);
    R = inversa*(1-d)/N*ones(N, 1);
    
endfunction