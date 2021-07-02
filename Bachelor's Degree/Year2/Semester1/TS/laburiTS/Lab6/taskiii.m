% in aceasta functie se explica ce s-a modificat
% o data cu cu aparitia defectului tehnic, cat se si
% analizeaza observabilitatea si controlabilitatea
% noului sistem (task iii)
function [] = taskiii()

    % preluam sistemul cu care se lucreaza
    load('CAR_TSA.mat');

    % modificam matricea B din cauza defectului tehnic
    B = [0 0; 0 0; 0 0.33];
    
    % s-a modificat linia a doua din matricea B (plina de 0-uri),
    % motiv pentru care viteza masinii in SR inertial
    % va fi egala cu 0 KM/h, intrucat:
    
    % x(t+1) = A * x(t) +  B * u(t)

    % in partea a doua a sumei de deasupra
    % reprezentata de produsul B * u(t), valoarea
    % de pe pozitia 2, corespunzatoare vitezei masinii
    % in SR inertial va fi egala cu 0.
    
    % prima parte a sumei reprezentata de produsul
    % A * x(t) va avea pe cea de a doua pozitie,
    % corespunzatoare vitezei masinii in SR inertial tot
    % 0 intrucat linia a doua din A, [0 -1/2 0], va modifica
    % strict valoarea vitezei anterioare (egala cu 0),
    % inmultind-o cu -1/2, deci obtinem o noua viteza
    % egala tot cu 0.
    
    % iesirea nu face altceva decat sa copieze ultima
    % stare, asa ca:
    % din primele 2 grafice ale figurii, se observa ca atat viteza,
    % cat si distanta parcursa este egala cu 0; Cum controlabilitatea
    % este influentata de B iar aceasta reprezentand elementele
    % de actionare, putem trage concluzia ca sistemul de accelerare
    % este defect.
   
    % pastram vectorul u de la subpunctele anterioare
    u = [100 24];
    
    % generam un model de stare folosind matricele
    % A, B (actual Bf), C, D
    sys = ss(A, B, C, D);
    
    % stabilim un vector de timp de dimensiune
    % convenabila pentru aplicatia noastra
    t = 0:0.1:30;
    
    % generam U astfel incat sa aibe atatea coloane
    % cat variabile de input si atatea linii cate
    % momente de timp pentru care se face analiza
    U = repmat(u, length(t), 1);
    
    % simulam raspunsul fortat al dinamicii sistemului
    Y = lsim(sys, U, t);
    
    numeFigura = 'Raspunsul fortat al dinamicii sistemului descris de (A, Bf, C, D)';
    
    afiseazaIesiri(Y, t, numeFigura);
    
    % sistemul nostru format din matricele
    % (A, Bf, C, D) nu este controlabil
    % intrucat rangul matricei R nu este
    % egal cu n, unde A este o matrice
    % de forma n x n (n = 3 in cazul nostru)
    
    if (verificaControlabilitate(A, B))
        disp("Sistemul descris de matricele (A, Bf, C, D) pe spatiul starilor este controlabil");
    else
        disp("Sistemul descris de matricele (A, Bf, C, D) pe spatiul starilor nu este controlabil");
    end
    
    
    % sistemul nostru format din matricele
    % (A, Bf, C, D) este observabil
    % intrucat rangul matricei Q este
    % egal cu n, unde A este o matrice
    % de forma n x n (n = 3 in cazul nostru)

    if (verificaObservabilitate(A, C))
        disp("Sistemul descris de matricele (A, Bf, C, D) pe spatiul starilor este observabil");
    else
        disp("Sistemul descris de matricele (A, Bf, C, D) pe spatiul starilor nu este observabil");
    end
    
end