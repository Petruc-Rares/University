function [x] = calculPI()
    m = 2020;
    x = 0;
    k = 0;
    while (k <= m) 
        x = x + (-1)^k/(2*k + 1);
        k = k +1;
    end
    x = x * 4;
    %{
    m = 1000;
    x = 0;
    for k = 0:m
       x = x + (-1)^k/(2*k+1);
    end
    x = x * 4;
    %}
end