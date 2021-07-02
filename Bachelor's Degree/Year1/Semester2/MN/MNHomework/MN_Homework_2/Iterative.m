% functie ce calculeaza vectorul PageRank
% R intr-un mod iterativ
function R = Iterative(nume, d, eps)
    
    % obtinem numarul de pagini si
    % matricea M
    [N, M] = getNM(nume);
    
    % numele fisierului de output
    output = numeFisOutput(nume);
    
    % obtinem vectorul PageRank
    R = getIterativePR(N, M, d, eps);
    
endfunction