% functie ce returneaza N, val1, val2
function[N, val1, val2] = getNval1and2(nume)

 fid = fopen(nume, 'r');
    
    % verificam daca s-a efectuat deschiderea cu succes    
    if fid == -1
        disp('Nu s-a putut deschide fisierul');
        return;
    endif
    

    N = fscanf(fid, "%d", 1);
    fseek(fid, 0, 'bof');
    linie = fgetl(fid);
    
    auxN = N;
    
    % citesc linie cu linie fisierul
    % pana ajung la linia corespunzatoare lui val1
    while auxN--    
        linie = fgetl(fid);        
    endwhile
    
    % ajuns aici, pot realiza citirea dorita
    val1 = fscanf(fid, "%f", 1);
    val2 = fscanf(fid, "%f", 1);
        
    fclose(fid);
    
endfunction