function[y, fs] = v()

    alpha = 45/50;
    [u, fs] = merge_sound_files("supernova.wav", "beep.wav", alpha);
    
    % afisam spectrul semnalului audio
    % inainte de aplicarea filtrului
    figure;
    plot_fft(u, fs);
    
    
    % am observat ca frecventa tiuitului are valoarea
    % 15999.4
    w0 = 2*pi*15999.4;
    
    Q = 0.1;
    
    % construim un filtru notch
    num = [1 0 w0^2];
    den = [1 w0/Q w0^2];
    
    HNotch = tf(num, den);
    
    y = apply_filter(HNotch, u, fs);

end