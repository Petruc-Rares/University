function[A, K] = buildAK(nume, fid, N)

    % parcurgem fiecare pagina din lista data
    while(N--) 
        
        NrPagina = fscanf(fid, "%d", 1);
        NrPaginiVec = fscanf(fid, "%d", 1);
        LegaturiPagini = dlmread(nume, ' ', [NrPagina 2 NrPagina 1+NrPaginiVec]);
        NrLinkuriAcPagina = length(find(LegaturiPagini == NrPagina));
        fseek(fid, NrPaginiVec*2, 'cof');
    
        A(NrPagina, LegaturiPagini) = 1;
        K(NrPagina, NrPagina) = NrPaginiVec - NrLinkuriAcPagina;
    
    endwhile
   
    % Facem aceasta operatie pentru ca ne dorim
    % ca A(i, i)) = 0; i = 1:N
    A = A - diag(diag(A));
    
    fclose(fid);

endfunction