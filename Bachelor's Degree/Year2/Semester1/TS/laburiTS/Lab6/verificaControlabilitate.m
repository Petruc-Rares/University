% aceasta functie determina daca sistemul
% reprezentat de matricele A si B este controlabil
function [eControlabil] = verificaControlabilitate(A, B)

    % A are dimensiunea n x n
    n = length(A);
    
    % implementare proprie in a calcula matricea
    % de controlabilitate R
    %
    %{
    % B are dimensiunea n x m
    m = size(B, 2);

    % primul bloc din matricea R este B
    R = B;
    
    for i=1:n-1
        crtBlock = R(:, (i - 1) * m + 1:i * m);
        newBlock = A * crtBlock;
        R = [R, newBlock];
    end
    %}
    
    
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