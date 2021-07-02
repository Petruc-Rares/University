% functie ce returneaza numarul de pivoti
% ai unei matrici
function [x] = nrPivoti(nl, nc)
    if (nl <= nc)
        x = nl - 1;
    else
        x = nc;
    endif
endfunction