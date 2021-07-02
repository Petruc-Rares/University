% aceasta functie determina daca sistemul
% reprezentat de matricele A si C este observabil
function [eObservabil] = verificaObservabilitate(A, C)

    % A are dimensiunea n x n
    n = length(A);
    
    % implementare proprie in a calcula matricea
    % de observabilitate Q
    %
    %{
    % C are dimensiunea p x n
    p = size(C, 1);

    % primul bloc din matricea Q este C
    Q = C;
    
    for i=1:n-1
        crtBlock = Q((i - 1) * p + 1:i * p, :);
        newBlock = crtBlock * A;
        Q = [Q; newBlock];
    end
    %}

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