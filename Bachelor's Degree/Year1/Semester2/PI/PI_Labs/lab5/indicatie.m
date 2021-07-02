function[y] = indicatie()
%citire date
[date,text]=xlsread('Note_stud.xls')
%extragere date =note studenti
x=date';
med_aritm=mean(x)%calcul valoare medie
estimatia=std(x) %estimatia=deviatia standard
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
y
figure(2); hist(xx)
title('Histograma cu datele finale')
end
%calcul medie aritmeticanoua si estimatia/deviatia standard noua %verificarea se repeta pana toate valorile sunt in intervalul ±3