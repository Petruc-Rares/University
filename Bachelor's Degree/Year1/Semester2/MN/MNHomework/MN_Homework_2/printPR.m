function[] = printPR(output, PR)

    fid = fopen(output, 'w');
    
    fprintf(fid, '%f\n', PR);    
        
    fclose(fid);

endfunction