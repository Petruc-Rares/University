function[] = sarcina_iii()

    clear all;
    close all;
    clc;

    t = [0:1:100];
    h = 100/3*exp(1).^(-t/3);
     
    u1 = sin(100*t);
    ts_continous_conv(u1, h, t);
    xlabel('t[s]');
    ylabel('y1[km/h]');
    legend('y1(t)');
    
    % la 16 secunde, viteza ajunge la 117 km/h
    % urmand ca aceasta sa nu mai creasca cu mult
    % lucru datorat de asimptota ce se observa
    % in grafic
    u2 = function_u(t, 0);
    ts_continous_conv(u2, h, t);
    xlabel('t[s]');
    ylabel('y2[km/h]');
    legend('y2(t)');

    % pentru pedala de acceleratie apasata pe
    % jumatate, viteza finala se reduce cu jumatate
    % graficul avand aceeasi forma asimptotica, astfel
    % automobilul atinge viteza finala dupa aproximativ
    % 16 secunde
    ts_continous_conv(u2/2, h, t);
    xlabel('t[s]');
    ylabel('y2_[km/h]');
    legend('y2_[t]');
    
end