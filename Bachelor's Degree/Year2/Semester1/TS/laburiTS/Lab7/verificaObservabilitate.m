% aceasta functie determina daca sistemul
% reprezentat de matricele A si C este observabil
function [eObservabil] = verificaObservabilitate(A, C)

    % A are dimensiunea n x n
    n = length(A);

    Q = obsv(A, C);
    rang = rank(Q);
    
    % verificam daca matricea Q este de rang maxim (monica)
    % sau nu
    if (rang == n) 
        eObservabil = true;
    else
        eObservabil = false;
    end

end