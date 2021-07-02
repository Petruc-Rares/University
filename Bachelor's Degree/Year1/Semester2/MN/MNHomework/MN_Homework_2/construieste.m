% am facut toata functia asta in loc sa inmultesc efectiv H cu A pentru ca
% operatiile de inmultire realizate asupra elementelor corespunzatoare
% de sub pivot se efectueaza degeaba(noi stim deja ca valorile de sub pivot sunt 0)
% iar in cazul matricilor mari am economisi timp prin apelarea acestei functii
function [x] = construieste(A, H, j)
    nl = size(A)(1);
    nc = size(A)(2);
    auxA = zeros(nl, nc);
    auxA = A;
    x = zeros(nl);
    % tot  ce e dupa coloana j in matricea A se calculeaza
    
    A(:, [j+1:nc]) = H * auxA(:, [j+1:nc]);
    
    %tot ce deasupra la pivot (sau pivotul) se calculeaza

    A([1:j], j) = H([1:j], :) * auxA(:, j);
    
    % tot ce e sub pivot este egal cu 0
    A(j+1:nl, j) = 0;
    
    x = A;
endfunction
