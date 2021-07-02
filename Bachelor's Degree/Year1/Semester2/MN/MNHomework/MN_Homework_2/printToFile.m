function[] = printToFile(nume, N, R1, R2, index, vector)

    output = numeFisOutput(nume);

    fid = fopen(output, 'w');
    
    fprintf(fid, '%d\n\n', N);
    fprintf(fid, '%f\n', R1);
    fprintf(fid, '\n');
    fprintf(fid, '%f\n', R2);
    fprintf(fid, '\n');
    
    for i = 1:size(R2)(1)
    
        fprintf(fid, "%d %d %f\n", i, index(i), vector(i));
    
    endfor
        
    fclose(fid);

endfunction