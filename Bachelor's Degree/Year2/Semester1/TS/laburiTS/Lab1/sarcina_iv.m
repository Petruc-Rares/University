function[] = sarcina_iv()
    
    t = 0:1:100;
    h = 100/3*exp(1).^(-t/3);
    
    u1 = sin(100*t);
    u2 = function_u(t, 0);
    u3 = u1 + u2;

    ts_continous_conv(u3, h, t);   
    xlabel('t[s]');
    ylabel('y3[km/h]');
    legend('y3(t)');
    
    figure;
    plot(t, u3);
    xlabel('t[s]');
    ylabel('u3[%]');
    legend('u3(t)');

    % stim de la curs ca pentru u1 si u2
    % doua semnale (u + v)(t) = u(t) + v(t)
    % lucru ce reiese si din graficele de mai
    % sus, rezultand astfel caracterul de sistem
    % linear al modelului automobilului
    
end