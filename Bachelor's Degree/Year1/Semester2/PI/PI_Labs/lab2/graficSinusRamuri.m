function [x] = graficSinusRamuri(t)
    if (sin(t) <= 0)
       x = sin(t);
    else 
        x = 0;
    end
end