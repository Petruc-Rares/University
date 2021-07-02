% functie ce returneaza N si M
function[N, M] = getNM(nume)

    % deschidem fisierul
    fid = fopen(nume, 'r');
    
    % verificam daca s-a efectuat deschiderea cu succes    
    if fid == -1
        disp('Nu s-a putut deschide fisierul');
        return;
    endif
    
    % citim datele de intrare
    % incepem cu numarul de pagini
    N = fscanf(fid, "%d", 1);
    
    A = zeros(N);
    K = zeros(N);
   
    % construim A, K
    [A, K] = buildAK(nume, fid, N);
    
    % calculam M corespunzator formulei de pe Wikipedia
    % folosesc inv pentru a calcula inversa lui K
    % pentru ca nu este costisitor sa faci inversa
    % unei matrici diagonale
    M = (inv(K)*A)';
    
endfunction