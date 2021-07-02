function [] = proba()
  graphics_toolkit('gnuplot');
function [res] = f(x)
    Rab = 1;
    Is = 3 * 10^(-12);
    vT = 0.025;
    res = Rab*Is*(e.^(x/vT)-1);
    %res = e.^x;
endfunction

function [res] = f2(x)

    res = -2-x;

endfunction

x = 2;

plot(x, f2(x));

hold on;
plot(x, f(x), 'r');
endfunction