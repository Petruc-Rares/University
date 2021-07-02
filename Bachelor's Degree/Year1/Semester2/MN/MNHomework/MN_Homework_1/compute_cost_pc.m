function [cost] = compute_cost_pc(points, centroids)

    % N reprezinta numarul de puncte
    N = size(points)(1);
    % D reprezinta numarul de dimensiuni ale
    % fiecarui punct din points/centroids
    D = size(centroids)(2);
    % NC reprezinta numarul de centroizi
    NC = (size)(centroids)(1);

    cost = 0;
    
    %pentru fiecare punct din points
        for i = 1:N
        
            minnorm = 1000;
            
            for j = 1:NC
            
                if (norm(points(i, :)- centroids(j, :)) < minnorm)
                
                    minnorm = norm(points(i, :)- centroids(j, :));
                
                endif
            
            endfor
            
            cost = cost + minnorm;
        
        endfor
      
endfunction

