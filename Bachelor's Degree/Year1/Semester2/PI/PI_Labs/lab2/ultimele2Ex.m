function [H] = ultimele2Ex(p, n)
    for i = 1:n
       if (p(i) < 0) || (p(i) > 1)
            disp("EROARE");
            return;
       end
    end
    H = 0;
    for i = 1:n
       H = H + p(i)*log2(p(i));
    end
    H = -H;
end