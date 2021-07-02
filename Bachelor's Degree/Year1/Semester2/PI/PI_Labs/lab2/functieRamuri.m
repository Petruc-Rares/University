function [f] = functieRamuri(t)
    if (t < 0)
        f = 0;
    else if ((0 <= t) && (t <= 1))
        f = t^2*(3-2*t);
    else
        f = 1;
    end
end