function task_4(A, B, C, D)

    % construim modelul
    % pe stare
    sys = ss(A, B, C, D);
    
    u = 20;
    q = 2;
    teta = 0;
    w = 0;
    
    x = [q teta u w]';
    
    t = [0:0.1:600];
    
    deltac = 2;
    deltath = 3;
    
    u = [deltac deltath];
    
    U = repmat(u, length(t), 1);
    
    % simulam raspunsul total al diniamicii
    % longitudinale
    Y = lsim(sys, U, t, x);
    
    figure;
    set(gcf,'name','Raspunsul total al dinamicii longitudinale');

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
    
end