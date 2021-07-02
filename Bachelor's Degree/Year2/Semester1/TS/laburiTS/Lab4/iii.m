function[y, fs] = iii()

    [u, fs] = play_file("bass_test.wav", false);

    num = [1 0 0];
    % cunoastem formula s^2 + 2*zeta*wn*s + wn^2
    % din calcule, reise simplu ca zeta este
    % aproximativ egal cu 0.05, motiv pentru care
    % si din diagrama Bode reiesind un overshoot
    % foarte mare
    
    % den = [1 62.83 394800];
    
    % astfel, am decis sa il stabilesc 
    % pe zeta cu valoarea aproximativ 1,
    % astfel nemaiexistand acel overshoot
     den = [1 1256.66 394800];
    
    H = tf(num, den);
    
    y = apply_filter(H, u, fs);
end