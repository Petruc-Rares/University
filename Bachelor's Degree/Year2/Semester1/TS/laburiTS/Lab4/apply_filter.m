function y = apply_filter(tf, u, fs)
    % pasul vectorului de timp
    T = 1/fs;
    
    n = length(u);
    
    % vectorul de timp
    t = [0: T: T*(n-1)];
    
    % aplicam functia de transfer
    % pe semnalul de intrare u
    y = lsim(tf, u, t);
    
    % se afiseaza diagrama Bode a filtrului
    % rezultat, conform indicatiei vi)
    figure;
    bode(tf);
    
    % se afiseaza spectrul semnalului audio
    % rezultat, conform indicatiei vi)
    figure;
    plot_fft(y, fs);
    
end