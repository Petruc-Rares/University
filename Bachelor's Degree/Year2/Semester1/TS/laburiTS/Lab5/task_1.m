function [H] = task_1(A, B, C, D)

    % mai intai construim modelul
    % pe stare
    sys = ss(A, B, C, D);
    
    % convertim modelul pe stare
    % la functia de transfer
    H = tf(sys);

    % Partea reala a polilor obtinuti din functia
    % de transfer este negativa, iar partea
    % reala a valorilor proprii din spectrul lui
    % A este tot negativa => sistemul e stabil
    
    % mai mult decat atat, obtinem aceleasi valori
    % atat pentru poli (dublii) a functiei de transfer
    % H, cat si pentru valorile proprii din spectrul
    % lui A
    pole(H)
    
    eig(A)
    
    
    % transformarea din model pe stare in functie
    % de transfer nu se poate face intotdeauna, intrucat
    % avem de a face cu o inversa de matrice, despre care
    % nu avem certitudinea ca e inversabila pentru orice
    % valoare a lui s
end