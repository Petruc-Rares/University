function [] = histograma()

    media=1;
    dispersia=0.5;
    N=1000;
    date=random('Norm', media, dispersia, 1, N);
    figure(1);
    hist(date)
    title('Histograma cu media=1, dispersia=0.5');


end