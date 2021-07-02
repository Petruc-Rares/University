% aceasta functie ne va returna pozitia masinii
% dupa 25 de secunde (exercitiul i, prima parte)
% cat si timpul dupa care se atinge temperatura
% de referinta in habitaclu (exercitiul i, a doua parte)
function [pozitie, timp]  = taski()

    % preluam sistemul cu care se lucreaza
    load('CAR_TSA.mat');
    
    % conform cerintei, u la momentul t = 0 
    % este un vector format din 100 si 24
    u = [100 24];
    
    % generam un model de stare folosind matricele
    % A, B, C, D
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
    
    numeFigura = 'Raspunsul fortat al dinamicii sistemului decris de(A, B, C, D)';
    
    afiseazaIesiri(Y, t, numeFigura);
    
    % stim ca pozitiile masinii in SR inertial
    % se afla pe prima coloana a matricei de iesiri
    % asa ca analizam primul grafic din figura, iar
    % din acesta reiese ca la secunda 25, autoturismul
    % a parcurs aproximativ 0.63889km
    
    pozitie = 0.63889;
    
    % cunoastem ca temperatura din habitaclul masinii
    % se afla pe a treia coloana a matricei de iesiri,
    % motiv pentru care analizam al treilea grafic din figura,
    % iar din acesta reiese ca dupa aproximativ 15 secunde,
    % temperatura tinde asimptotic la valoarea de 24 de grade celsius;
    % undeva la 18 secunde, temperatura depaseste deja 23.9 grade si
    % mai creste foarte putin si foarte incet, motiv pentru care
    % returnez acest moment de timp
    
    timp = 18;
    
end