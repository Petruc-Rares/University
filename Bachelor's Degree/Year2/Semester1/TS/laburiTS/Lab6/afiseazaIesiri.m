% aceasta functie este folositoare in afisarea
% iesirilor pentru sistemele ce se formeaza in
% cadrul aplicatiei de laborator cu numarul 6
function[] = afiseazaIesiri(Y, t, numeFigura)

    figure;
    set(gcf,'name', numeFigura);
    
    subplot(3, 1, 1);
    y1 = Y(:, 1);
    plot(t, y1);
    title('Pozitia masinii in SR inertial (GPS)')
    xlabel('t [sec]');
    ylabel('y1 [km]');
    legend('y1(t)');
    
    subplot(3, 1, 2);
    y2 = Y(:, 2);
    plot(t, y2);
    title('Viteza masinii in SR inertial (ENCODER ROATA)')
    xlabel('t [sec]');
    ylabel('y2 [km/h]');
    legend('y2(t)');
    
    subplot(3, 1, 3);
    y3 = Y(:, 3);
    plot(t, y3);
    title('Temperatura din habitaclul masinii (SENZOR TEMPERATURA)')
    xlabel('t [sec]');
    ylabel('y3 [C*]');
    legend('y3(t)');

end