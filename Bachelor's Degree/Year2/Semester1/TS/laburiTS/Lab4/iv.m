function[y, fs] = iv()

    [u, fs] = play_file("supernova.wav", false);

    plot_fft(u, fs);
    % putem obtine un filtru trece banda prin inserierea
    % unui filtru trece sus (pentru frecvente mai mari de 1Khz)
    % cu un filtru trece jos (pentru frecvente mai mici de 5Khz)

    % presupunem ca avem o valoare a rezistentei
    % egala in ambele filtre
    R = 1000;
    
    fL = 1000;
    
    CH = 1/(2*pi*fL*R);
    
    numH = [R*CH 0];
    denH = [R*CH 1];
   
    HH = tf(numH, denH); 
   
    fH = 5000;
    CL = 1/(2*pi*fH*R);

    numL = [0 1];
    denL= [R*CL 1];
    
    HL = tf(numL, denL);
    
    HBand = HH*HL;
    % vrem sa vedem diagrama Bode si cand este
    % lipsita de amplificare
    y_aux = apply_filter(HBand, u, fs);
    % trebuie sa amplificam plaja de frecvente
    % cu 5 decibeli; cunoastem formula amplificarii
    % amplificare = 20 * lg(k) => in cazul nostru
    % 5 = 20 * lg(k) => k aproximativ egal 1.778.
    
    % astfel o valoarea a lui k egala cu 2 satisface
    % conditia impusa de cerinta, dupa cum se poate
    % observa si din diagramele Bode pentru cele 2 functii
    % de transfer
    HBandAmplified = HBand*2;
    y = apply_filter(HBandAmplified, u, fs);  
    
end