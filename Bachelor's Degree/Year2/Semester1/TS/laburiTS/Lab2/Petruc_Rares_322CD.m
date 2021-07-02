function [] = Petruc_Rares_322CD()
   s = tf('s');
   % num = 100
   num = [0 100];
   % den = 2*s + 1
   den = [2 1];
   
   % H = 100/(2*s + 1)
   H = tf(num, den);
   
   % i) s = -1/2 => vom avea o iesire de
   % forma C*e^(-1/2*t), unde C este o constanta
   % iar astfel iesirea sistemului va fi amortizata
   % deci sistemul este stabil (puteam spune direct
   % ca daca partea reala a lui s este negativa
   % sistemul este stabil)

   
   % ii) adaug un pol ce are partea reala pozitiva
   % 3s - 2 = 0 => s = 2/3 > 0 => sistem instabil
   % 6*s^2 - s - 2 = (2*s + 1)*(3*s - 2)
   denInstabil = [6 -1 -2];
   H1 = tf(num, denInstabil);

   rsp_treaptaStabil = step(H);
   figure;
   plot(rsp_treaptaStabil);
   title('Raspunsul la treapta in cazul sistemului stabil');
   xlabel('Timp');
   ylabel('Amplitudine');
   
   rsp_treaptaInstabil = step(H1);
   figure;
   plot(rsp_treaptaInstabil);
   title('Raspunsul la treapta in cazul sistemului instabil');
   xlabel('Timp');
   ylabel('Amplitudine');
   
   % iii) asa cum ma asteptam, in urma
   % adaugarii unui pol cu partea reala pozitiva
   % obtinem o iesire infinita, pe cand in urma
   % folosirii primei functii de transfer
   % am obtinut un sistem stabil.
   
   % iv)
   rsp_impulsStabil = impulse(H);
   figure;
   plot(rsp_impulsStabil);
   title('Raspunsul la impuls in cazul sistemului stabil');
   xlabel('Timp');
   ylabel('Amplitudine');
   
   % v) construiesc graficul din poza u(t)
   t = 0:0.1:10;
   u = zeros(size(t));
   for i = 1:length(t)
       if ((t(i) >= 0) && (t(i) <= 1))
           u(i) = 1;
       elseif((t(i) >= 2) && (t(i) <= 3))
           u(i) = 1;
       elseif((t(i) >= 4) && (t(i) <= 5))
           u(i) = 1;
       end
   end
   
   rsp_semnal = lsim(H, u, t);
   figure;
   plot(rsp_semnal);
   title('Raspusul la trenul de impulsuri');
   xlabel('Timp');
   ylabel('Amplitudine');

end