% functie ce primeste ca parametri rezistenta
% de sarcina, rezistenta retelei pasiviziate si valoarea
% tensiunii de mers in gol
% si returneaza valoare puterii maxime
% si rezistenta pentru care se obtine aceasta valoare
function[P_max, Rab]=grafice(RAB, RAB0, UAB0)

    % generam o gama de valori convenabile
    % pentru rezistenta
    R = linspace(0,4,400);
    
    % scriem formula curentului
    % functie de rezistenta
    % si plotam graficul
    I = UAB0./(R+RAB0);
    plot(R, I);
    
    % avem grija sa nu se stearga
    % graficul la inserarea altuia
    hold on
    
    % scriem formula tensiunii la borne
    % si plotam graficul
    U = R./(R +RAB0)*(UAB0);
    plot(R, U);
   
    % scriem formula puterii absorbita de
    % rezistor si plotam graficul
    P = (R./((R+RAB0).^2))*(UAB0)^2;
    plot(R, P);
   
    % calculam P_max ce reiese din grafic
    % si indexul corespunzator din gama
    % de valori oferite corespunzatoare
    % valorii rezistentei, pentru care puterea
    % este maxima
    [P_max, index] = max(P);
    
    % rezistenta pentru care puterea e maxima
    % (nu este cea exacta, intrucat gama de
    % valori furnizata este relativ mica, insa
    % se apropie de valoarea reala (1 ohm))
    Rab = R(index);
    
    % Stim ca U = E - i*RAB0
    
    % in cazul meu tensiune electromotoare
    % este egala cu tensiunea in gol trimisa
    % ca parametru functiei
    E = UAB0;
    
    % dau un set de valori convenabile petru 
    % curent
    i = [-3:1:0.1];
    
    % reprezint grafic tensiunea functie de celelalte
    % tensiuni din circuit prin aplicarea Kirch. II
    u1 = UAB0 - i*RAB0;
    plot(i, u1);
    
    % reprezint grafic tensiunea la bornele rezistorului
    u2 = i*RAB;
    plot(i, u2);
    
    % din calcul simplu reiese
    % ca punctul de intersectie al graficelor u1 si u2
    % este (-1, -1) pe care il si marcam in reprezentare
    % cu o steluta albastra
    plot(-1, -1, 'b*');
    
    % calculam puterea pentru rezistenta initiala
    % si marcam punctul corespunzator de pe grafic
    % cu o steluta verde
    Pi = (RAB/((RAB+RAB0)^2))*(UAB0)^2;
    plot(RAB, Pi, 'g*');

    % marcam punctul pentru care puterea este maxima
    plot(Rab, P_max, 'r*');
    
    % marcam legendele celor 5 grafice trasate    
    legend('I(R)', 'U(R)', 'P(R)', 'U=E-RABO*i', 'U = f(i)');
    
end