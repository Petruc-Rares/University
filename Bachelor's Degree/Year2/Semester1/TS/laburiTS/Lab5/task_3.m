function task_3(A, B, C, D)

    % construim modelul
    % pe stare
    sys = ss(A, B, C, D);

    deltac = 2;
    deltath = 3;
    
    t = 0:0.1:600;
    
    % construim vectorul intrarilor
    u = [deltac deltath];
    % U must be a matrix with as many rows as
    % samples in the time vector T, and as many columns as
    % input channels.
    U = repmat(u, length(t), 1);
    
    % simulam raspunsul fortat al dinamicii longitudinale
    % a avionului
    Y = lsim(sys, U, t);
    
    figure;
    set(gcf,'name','Raspunsul fortat al dinamicii longitudinale');
    
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