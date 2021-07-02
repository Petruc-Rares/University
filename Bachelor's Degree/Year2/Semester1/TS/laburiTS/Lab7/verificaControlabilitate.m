% aceasta functie determina daca sistemul
% reprezentat de matricele A si B este controlabil
function [eControlabil] = verificaControlabilitate(A, B)

    % A are dimensiunea n x n
    n = length(A);
    
    R = ctrb(A, B);
    rang = rank(R);
    
    % verificam daca matricea R este de rang maxim (epica)
    % sau nu
    if (rang == n) 
        eControlabil = true;
    else
        eControlabil = false;
    end

end