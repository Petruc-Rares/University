% in aceasta functie se explica ce s-a modificat
% o data cu cu aparitia defectului tehnic, cat se si
% analizeaza observabilitatea si controlabilitatea
% noului sistem (task iv)
function [] = taskiv()

    % preluam sistemul cu care se lucreaza
    load('CAR_TSA.mat');

    % modificam matricea C din cauza defectului tehnic
    C = [1 0 0; 0 1 0; 0 0 0];
    
    % s-a modificat linia a treia din matricea C (plina de 0-uri),
    % motiv pentru care temperatura va fi constant
    % egala cu 0 C* conform senzorului de temperatura, intrucat:
    
    % y(t) = C * x(t) + D * u(t)
       
    % Deoarece matricea C este inmultita
    % cu vectorul starilor (x) si adunata cu un
    % vector care da 0 mereu in cazul sistemului nostru
    % din cauza lui D (care are numai 0-uri)
    % pentru a calcula iesirile, pozitia a treia
    % din vectorul iesirilor ce corespunde temperaturii
    % habitaclului din masina va avea o valoare egala cu 0
    
    % din cel de al treilea grafic al figurii, se observa
    % clar cum temperatura ar fi egala constant cu 0 C*
    % in plus observabilitatea este influentata de matricea
    % C, aceasta reprezentand senzorii, deci
    % putem trage concluzia ca s-a defectat senzorul de temperatura.
    
    % pastram vectorul u de la subpunctele anterioare
    u = [100 24];
    
    % generam un model de stare folosind matricele
    % A, B, C (actual Cf), D
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
    
    numeFigura = 'Raspunsul fortat al dinamicii sistemului descris de (A, B, Cf, D)';
    
    afiseazaIesiri(Y, t, numeFigura);
    
    % sistemul nostru format din matricele
    % (A, B, Cf, D) este controlabil
    % intrucat rangul matricei R este
    % egal cu n, unde A este o matrice
    % de forma n x n (n = 3 in cazul nostru)
    
    if (verificaControlabilitate(A, B))
        disp("Sistemul format din matricele (A, B, Cf, D) este controlabil");
    else
        disp("Sistemul format din matricele (A, B, Cf, D) nu este controlabil");
    end
    
    
    % sistemul nostru format din matricele
    % (A, B, Cf, D) nu este observabil
    % intrucat rangul matricei Q nu este
    % egal cu n, unde A este o matrice
    % de forma n x n (n = 3 in cazul nostru)

    if (verificaObservabilitate(A, C))
        disp("Sistemul descris de matricele (A, B, Cf, D) este observabil");
    else
        disp("Sistemul descris de matricele (A, B, Cf, D) nu este observabil");
    end
    
end