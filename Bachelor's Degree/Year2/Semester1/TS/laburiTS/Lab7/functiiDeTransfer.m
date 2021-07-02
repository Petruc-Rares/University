function [] = functiiDeTransfer()

    % num = 1
    num = [0 0 1];
    % den  = 0.7 * s^2 + 0.1 * s + 0.3
    den = [0.7 0.1 0.3];
    
    % H = 1 / (0.7 * s^2 + 0.1 * s + 0.3)
    H = tf(num, den);

    % cunoastem forma generala a numitorului
    % unui sistem de ordinul al 2-lea:
    % (s^2 + 2 * zeta * wn * s + wn^2)
    % motiv pentru care impartim si la
    % numarator si la numitor cu 0.7 ca sa obtinem
    % un numitor care sa aibe aceasta forma generala
    
    % obtinem numitor egal cu:
    % s^2 + 1/7*s + 3/7
    % de aici reiese wn^2 = 3/7 => wn = 0.65 (aproximativ)
    % cum 2 * zeta * wn = 1/7 => zeta = 1/(14 * wn)
    % astfel zeta = 0.11 (aproximativ)
    wn = sqrt(3/7);
    zeta = 1/(14*wn);
    
    % in ceea ce priveste stabilitatea,
    % analizam polii sistemului si observam
    % ca amandoi au partea reala strict negativa,
    % deci sistemul este stabil BIBO in sens strict
    P = pole(H);
    
    e = exp(1);
    sigma = e^(-pi*zeta)/sqrt(1-zeta^2);
    
    % din aplicarea formulei, reiese ca sigma (suprareglajul pentru
    % treapta)
    % este aproximativ egal cu 0.7141, valoare aproximativa
    % la care putem ajunge si din formula (ymax - yp)/yp
    % ale caror valori le puteam lua din grafic
    % ymax = 5.6941, ypermanent=3.3333 cu aproximatie
    t = [0:0.1:160];
    u = [t <= 150] - [t > 152];
    rsp_semnal = lsim(H, u, t);
    
    figure;
    set(gcf,'name', "raspunsul la treapta al sistemului nostru");
    plot(t, rsp_semnal);
    title('Raspunsul la treapta al sistemului')
    xlabel('t [sec]');
    ylabel('y');
    legend('y(t)');
    
    % asa cum reiese si din formula aplicata mai sus, suprareglajul
    % este influentat de factorul de amortizare (zeta)

    % u(t) = sin(w*t), pentru w = wn vom obtine
    % amplificarea maxima a semnalului de intrare,
    % lucru ce iese din diagrama bode a sistemului nostru
    
    t = [0:0.1:160];
    u = sin(t*wn);
    rsp_semnal = lsim(H, u, t);
    
    figure;
    plot(t, rsp_semnal);
    set(gcf,'name', "raspunsul la treapta al sistemului nostru");
    plot(t, rsp_semnal);
    title('Raspunsul la sinusoida de pulsatie wn')
    xlabel('t [sec]');
    ylabel('y');
    legend('y(t)');
    
    
    % este vorba de un filtru jos, intrucat recunoastem formula generala
    % a functiei de transfer de forma: K * w0^2/(s^2 + w0/Q*s + w0^2)
    % mai mult decat atat daca inlocuim s cu jw, observam ca pentru
    % frecvente mici numitorul tinde la w0^2 deci raportul va tinde la K,
    % iar pe cazul in care avem frecvente foarte mari numitorul tinde la
    % infinit, rezultand astfel raportul egal cu 0.
    
    figure;
    bode(H)
    
    % din diagrama Bode, vedem ca se intampla cu adevarat ce am anticipat
end