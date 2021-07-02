% aceasta functie verifica daca sistemul este
% controlabil si daca este observabil (taskul 2)
function [] = taskii()

    % preluam sistemul cu care se lucreaza
    load('CAR_TSA.mat');
    
    % sistemul nostru este controlabil
    % intrucat rangul matricei R este
    % egal cu n, unde A este o matrice
    % de forma n x n (n = 3 in cazul nostru)
    
    if (verificaControlabilitate(A, B))
        disp("Sistemul descris de matricele (A, B, C, D) pe spatiul starilor este controlabil");
    else
        disp("Sistemul descris de matricele (A, B, C, D) pe spatiul starilor nu este controlabil");
    end
    
    
    % sistemul nostru este observabil
    % intrucat rangul matricei Q este
    % egal cu n, unde A este o matrice
    % de forma n x n (n = 3 in cazul nostru)
    
    if (verificaObservabilitate(A, C))
        disp("Sistemul descris de matricele (A, B, C, D) pe spatiul starilor este observabil");
    else
        disp("Sistemul descris de matricele (A, B, C, D) pe spatiul starilor nu este observabil");
    end
    
end