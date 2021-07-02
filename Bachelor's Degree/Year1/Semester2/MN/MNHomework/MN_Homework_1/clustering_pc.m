function [centroids] = clustering_pc(points, NC)

    N = size(points)(1);
    D = size(points)(2);
    
    centroids = zeros(0, 0);
    prev_centroids = centroids;
    clusters = zeros(0, 0);
    
    contor = 1;
    
    while (contor < N)
    
        clusters = [clusters, points(contor:contor + (NC - 1), 1:D)];

        contor = contor + NC;
        
    endwhile
    
    k = 1;
    suma = 0;
    
    %pentru fiecare linie din cluster
    for i = 1:NC
    
       medium = zeros(0, 0);
       
       k = 1;
    
       % se face media aritmetica pentru fiecare dimensiune din cele D
       while (k <= D)
       
           suma = 0;
       
           %se sare practic din D in D dimensiuni
           for j = k:D:D*(N/NC)
       
               suma = suma + clusters(i, j);
       
           endfor
           
           suma = suma / (N/NC);
           
           medium = [medium, suma];
           
           k++;
           
       endwhile
       
       centroids = [centroids; medium];
    
    endfor

    s = zeros(NC, D + 1);
    prev_centroids = centroids;

    while 1

            s = zeros(NC, D + 1);

        for contor = 1:N
    
            minnorm = 1000000;
            auxi = 0;
        
            %for i = 2:NC
        
            for i = 1:NC
        
                if (norm(prev_centroids(i, :) - points(contor, :)) < minnorm)
            
                    minnorm = norm(prev_centroids(i, :) - points(contor, :));
                    auxi = i;
            
                endif
        
            endfor
                        
            s(auxi, 1:D) = s(auxi, 1:D) + points(contor, :);
            s(auxi, D + 1) = s(auxi, D + 1) + 1;
        
        endfor
       
       for i = 1:NC
       
           if (s(i, D+1) != 0)
           
               s(i, 1:D) = s(i, 1:D) / s(i, D+1);
           
           endif
   
       endfor
  
       centroids = s(:, 1:D);
  
       if (prev_centroids == centroids) 
       
           break;
       
       endif
       
       prev_centroids = centroids;
  
   
   endwhile
   %disp(centroids);

endfunction
