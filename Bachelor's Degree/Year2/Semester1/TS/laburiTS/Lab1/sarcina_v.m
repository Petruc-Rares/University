function [] = sarcina_v()

    t = 0:1:100;
    h = 100/3*exp(1).^(-t/3);
    
    u4 = function_u(t, 3);
    ts_continous_conv(u4, h, t);
    
    xlabel('t[s]');
    ylabel('y4[km/h]');
    legend('y4(t)');
    
    figure;
    xlabel('t[s]');
    ylabel('u4[%]');
    legend('u4(t)');
    
    % putem observa din graficul y4(t), cum
    % in urma raspunsului la treapta 
    % intarziata, graficul este translatat
    % cu 3 secunde la dreapta (mai tarziu)
    % fata de cel rezultat cu treapta fara intarziere
    % rezultand astfel caracterul de sistem invariant
    % in timp al modelului automobilului
    
end