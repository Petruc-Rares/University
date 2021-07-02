function task_2(A, B, C, D)

    % construim modelul
    % pe stare
    sys = ss(A, B, C, D);

    u = 20;
    q = 2;
    teta = 0;
    w = 0;
    
    % construim vectorul starilor
    x = [q teta u w]';
    
    % dam valori convenabile vectorului de timp
    % cat sa ajungem la regiunea stationara
    
    t = [0:0.1:600];
    
    % simulam raspunsul liber al dinamicii
    % longitudinale
    [Y, t, x] = initial(sys, x, t);
    
    figure;
    set(gcf,'name','Raspunsul liber al dinamicii longitudinale');
    
    subplot(3, 1, 1);
    q = Y(:, 1);
    plot(t, q);
    title('Rata de tangaj functie de timp')
    xlabel('sec');
    ylabel('grd/sec');
    legend('q(t)');
    
    subplot(3, 1, 2);
    zPunct = Y(:, 2);
    plot(t, zPunct);
    title('Viteza pe axa OZ a SR atasat Pamantului functie de timp')
    xlabel('sec');
    ylabel('m/sec');
    legend('ZPunct(t)');
    
    
    subplot(3, 1, 3);
    v = Y(:, 3);
    plot(t, v);
    title('Viteza totala a aeronavei functie de timp')  
    xlabel('sec');
    ylabel('m/sec');
    legend('v(t)')
    
    % In final, se observa simplu din graficele
    % de la ultimele 3 subpuncte ca raspunsul total
    % reprezinta insumarea raspunsului liber si a
    % celui fortat.
    
end