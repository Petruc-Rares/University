function [] = polarizareInversa(UAB0, RAB0, Is, Vt)

    function[res]=f(u, UAB0, RAB0)
        res = UAB0/RAB0 - u./RAB0;
    end

    function[res]=f2(u, Is, Vt)
       res = Is*(1- exp(1).^(-u/Vt)); 
    end

    % u se masoara in miliVolti
    % i se masoara in picoAmperi
    u = linspace(-0.72, 0.68, 200);
    plot(u, f(u, UAB0, RAB0));
    hold on
    plot(u, f2(u, Is, Vt));
    xlabel('u(mV)')
    ylabel('i(pA)')
    
    txt = 'dreapta de sarcina \rightarrow';
    text(-1.7, -1.5, txt, 'HorizontalAlignment', 'left')
    
    legend('i=J-Gu', 'i=g(u)');
    txt = 'dreapta de sarcina \rightarrow';
    text(-0.6, -2, txt, 'HorizontalAlignment', 'left')
    
end

