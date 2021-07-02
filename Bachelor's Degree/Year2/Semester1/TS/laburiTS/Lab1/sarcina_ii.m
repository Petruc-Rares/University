% functia triunghiular nu primeste
% nici un parametru si intoarce
% un impuls triunghiular
function [] = sarcina_ii()

    clear all;
    close all;
    clc;
    
    a = -1;
    b = 1;
    
    x = a:0.01:b;
    y = 1 - abs(x);
    plot(x, y, 'Color', 'b');
    
    hold on;
    
    x = a-abs(a/2):0.01:a;
    y = repmat(min(y), 1, length(x));
    plot(x, y, 'Color', 'b');
    
    x = b:0.01:b+abs(b/2);
    y = repmat(y(1), 1, length(x));
    plot(x, y, 'Color', 'b');
    
    % stabilim limite favorabile pentru o
    % vizualizare buna a semnalului
    xlim([a-abs(a/2), b+abs(b/2)]);
    ylim([y(1) - 0.2, 1.2]);
    
    xlabel('t[s]');
    ylabel('trian(t)');
    legend('trian(t)');
    
end

