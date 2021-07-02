function [z2, xx] = ex32(Tensiuni, vnalpha)

    x = sort(Tensiuni);
  
    z2=[];
    xx=[];
    
    i=1;

        while 1
        
        mv=mean(x);
        delta=std(x);
        
        v = (mv - x(i))/delta;
    
        if (abs(v) > vnalpha)
           z2 = [z2, x(i)];
           x(i) = [];
        else 
            xx = [xx, x(i)];
        end
        if (abs(v) < vnalpha)
           xx = [xx, x(i:length(x))']; 
           break; 
        end
        
        i= i+1;
        end
      
end