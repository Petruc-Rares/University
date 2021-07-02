function [sol] = hsvHistogram(path_to_image, count_bins)

    HSVMatrix = imread(path_to_image);
    
    % MODIFICAM R, G, B de peste tot in R', G', B'
    
    HSVMatrix = double(HSVMatrix)/double(255);
       
    [nr nc] = size(HSVMatrix);
    
    for j = 1:nc/3
    
        HSVMatrix(:, [j, nc/3 + j, 2*nc/3 + j]) = conversie(HSVMatrix(:, [j, nc/3 + j, 2*nc/3 + j]));
    
    endfor

    binranges = [0:1.01/count_bins:1.01];    
    
    sol = histc(HSVMatrix(:, :, 1)(:), binranges)'(1:count_bins);
    sol = [sol, histc(HSVMatrix(:, :, 2)(:), binranges)'(1:count_bins)];
    sol = [sol, histc(HSVMatrix(:, :, 3)(:), binranges)'(1:count_bins)];
endfunction

function [HSVMatrix] = conversie(A)

    % determinam coloana cu termenii maximi de pe fiecare linie

    HSVMatrix = zeros(size(A)(1), 3);
    maxcol = (max(A'))';
    mincol = (min(A'))';
    delta = maxcol - mincol;

    % in acest caz modificam elementele din A
    % de pe pozitia i si 1 (corespunzatoare HUE)

    

    % se face saturation functie de cmax

    % caz cmax = 0
    [i, j] = find(maxcol == 0);
    HSVMatrix(i, 2) = 0;

    % caz cmax != 0
    [i, j] = find(maxcol != 0);
    HSVMatrix(i, 2) = delta(i, 1)./maxcol(i, 1);
    
    % value

    HSVMatrix(:, 3) = maxcol(:, 1);

    % delta = 0
    [i, j] = find(delta == 0);

    HSVMatrix(i, 1) = 0;
    maxcol(i, 1) = -1;
    
    % delta != 0
    
      
    % caz Cmax = R' 
    % i1 reprezinta indicele liniilor in dreptul
    % carora se gaseste egalitatea ceruta
    
    [i1, j1] = find(maxcol(:, 1) == A(:, 1));
    
    % eliminam din i1, cazurile de delta = 0
    
    HSVMatrix(i1, 1) = 60*mod((A(i1, 2) - A(i1, 3))./delta(i1, 1), 6);
    
    % initializam restul coloanelor la -1 pentru
    % a nu fi verificate conditii urmatoare
    maxcol(i1, 2) = -1;
    maxcol(i1, 3) = -1;

    % caz Cmax = G'
    [i1, j1] = find(maxcol(:, 1) == A(:, 2));
    
    HSVMatrix(i1, 1) = 60* ((A(i1, 3) - A(i1, 1))./delta(i1, 1) + 2);    

    % initializam coloana de B' la -1 pentru
    % a nu fi verificate conditii urmatoare
    maxcol(i1, 3) = -1;
    
    % caz Cmax = B'
    [i1, j1] = find(maxcol(: , 1) == A(:, 3));
    
    % eliminam din i1, cazurile de delta = 0
    
    HSVMatrix(i1, 1) = 60* ((A(i1, 1) - A(i1, 2))./delta(i1, 1) + 4);  
    
    HSVMatrix(:, 1) = HSVMatrix(:, 1)/360;

endfunction