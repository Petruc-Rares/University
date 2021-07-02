% functia dreptunghiular primeste ca parametri
% doua variabile de tip intreg si afiseaza
% semnalul dreptunghiular drept(t), unde a <= t <=b
function [] = sarcina_i(a, b)

    if (a > b)
       disp("a trebuie sa fie mai mic decat b");
       return;
    end
    
    x = a:0.01:b;
    y = ones(size(x));
    plot(x, y, 'Color', 'b');
    
    hold on;
    
    x = a-abs(a/2):0.01:a;
    y = zeros(size(x));
    plot(x, y, 'Color', 'b');
    
    x = b:0.01:b+abs(b/2);
    y = zeros(size(x));
    plot(x, y, 'Color', 'b');
    
    plot([a, a], [0, 1], 'Color', 'b');
    plot([b, b], [0, 1], 'Color', 'b');
    
    % stabilim limite favorabile pentru o
    % vizualizare buna a semnalului
    xlim([a-abs(a/2), b+abs(b/2)]);
    ylim([-0.2, 1.2]);
    
    xlabel('t[s]');
    ylabel('drept(t)');
    legend('drept(t)');
    
end

