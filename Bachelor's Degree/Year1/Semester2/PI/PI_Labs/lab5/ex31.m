function[y, xx, med_aritm] = ex31()
%citire date
[date, temp] = xlsread('Date_temp.xls');
%extragere date =note studenti
x=date';
med_aritm=mean(x);%calcul valoare medie
estimatia=std(x); %estimatia=deviatia standard
%creare histograma
figure(1); hist(x)
title('Histograma cu datele initiale')
%testul 3sigma
y=[]; xx=[];
dimensiune=length(x)
for i=1:dimensiune
    if x(i)>med_aritm+3*estimatia
        y=[y x(i)];
    elseif x(i)<med_aritm-3*estimatia
        y=[y x(i)];
    else
        xx=[xx x(i)];
        dimensiunex=length(xx);
    end
end
figure(2); hist(xx)
title('Histograma cu datele finale')
y
end