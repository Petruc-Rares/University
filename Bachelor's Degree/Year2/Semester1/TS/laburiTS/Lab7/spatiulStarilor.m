function [] = spatiulStarilor()

    A = [-51 15 8; 12 10 2; -8 12 -80];
    B = [0.2 0; 0.4 1; 0 0];
    C = [1 4 0; 0 15 0];
    D = [0 0; 0 0];

    % stim ca A este de dimensiunea n x n
    % unde n reprezinta numarul variabilelor
    % de stare, rezulta ca noi avem 3 stari
    
    % B este de dimensiunea n x m, unde m
    % reprezinta numarul de variabile de intrare
    % rezulta ca noi avem 2 intrari
    
    % C este de dimensiune p x n, unde p
    % reprezinta numarul de variabile de iesire,
    % deci noi avem 2 iesiri
    
    eig(A);
    % din valorile proprii rezultate a lui A,
    % se poate observa o valore proprie (13.1806),
    % care nu se afla in semiplanul complex stang
    % motiv pentru care sistemul nu e stabil
    
    % generam un model pe stare
    sys = ss(A, B, C, D);
    
    % stabilim intrari treapta
    u = [1 1];
    
    % stabilim un vector de timp de dimensiune
    % convenabila pentru aplicatia noastra
    t = 0:0.1:50;
    
    % generam U astfel incat sa aibe atatea coloane
    % cat variabile de input si atatea linii cate
    % momente de timp pentru care se face analiza
    U = repmat(u, length(t), 1);
    
    % simulam raspunsul fortat al dinamicii sistemului
    Y = lsim(sys, U, t);
    plot(t, Y(:, 1));
    set(gcf,'name', "raspunsul la treapta al sistemului nostru");
    title('Raspunsul la treapta al sistemului')
    xlabel('t [sec]');
    ylabel('y1');
    legend('y1(t)');
    
    % stim de la curs ca yt(t) = suma de la i la k
    % din Ai*e^(pi*t), unde k reprezinta numarul polilor.
    % Avand un pol ce se afla in semiplanul
    % complex drept reiese ca timpul tranzitoriu tinde la infinit,
    % motiv pentru care componenta permanenta nu mai
    % este evidentiata pe grafic.
    
    if (verificaControlabilitate(A, B))
        disp("Sistemul descris de matricele (A, B, C, D) este controlabil")
    else
        disp("Sistemul descris de matricele (A, B, C, D) nu este controlabil")
    end
    
        
    if (verificaObservabilitate(A, C))
        disp("Sistemul descris de matricele (A, B, C, D) este observabil")
    else
        disp("Sistemul descris de matricele (A, B, C, D) nu este observabil")
    end
    
    % am folosit codul din indrumar, insa am dat noi valori
    % pentru spectrele matricelor (A + B*F), respectiv (A + L*C)
    % astfel incat valorile proprii din spectrul de estimare sa fie
    % mult mai mari decat valorile proprii din spectrul de control,
    % lucru necesar pentru ca urmatoarea comanda data sa aibe cea mai
    % proaspata estimare a starii cu putinta. In acelasi timp, am avut grija
    % ca valorile proprii sa se afle in semiplanul complex stang, ca sa
    % avem un sistem stabil.
    
     % dimensiunile matricelor reprezentarii pe stare
    [n,n] = size(A);
    [n,m] = size(B);
    [p,n] = size(C);
    % Pasul 0
    % conditia i este indeplinita
    R = ctrb(A,B);rank(R);
    Q = obsv(A,C);rank(Q);

    % conditia ii este indeplinita
    rank([-A -B;C zeros(p,m)]);
    
    % Pasul 1
    Atilda = [A zeros(n,p);-C zeros(p,p)];
    Btilda = [B; zeros(p,m)];
    Ctilda = [zeros(p,n) eye(p)];

    Fext = place(Atilda, Btilda, [-23.31 + j, -23.31 - j, -19.732, -19.732 -20.315]);
    Fext = -Fext;
    F = Fext(:,1:n);
    Ftilda = Fext(:,n+1:n+p);
    
    % Pasul 2
    L = place(A',C',[-123432124, -123231256, -424141223]);
    L=-L';
    
    % Pasul 3
    A1 = A+L*C+B*F;
    A2 = B*Ftilda;
    [l1,c1] = size(A1);
    [l2,c2] = size(A2);
    
    Ak = [A1 A2;zeros(c1+c2-l1,c1) zeros(c1+c2-l1,c2)];
    Bk = [L; eye(size(L,2))];
    Ck = Fext;
    Dk = zeros(size(Ck,1),size(Bk,2));
    
    % Formarea buclei de reactie
    K = ss(Ak,Bk,Ck,Dk);
    T = ss(A,B,C,D);
    H0 = feedback(series(K,T),eye(2));
    figure;
    step(H0,10)
    
end