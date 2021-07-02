function u = function_u(t, origin)
    u = zeros(size(t));

    for i = 1:length(t)
       
        if t(i) > origin
           
            u(i) = 1;
            
        end
        
    end

end