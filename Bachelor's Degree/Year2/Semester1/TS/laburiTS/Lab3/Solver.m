classdef Solver
    methods
        % 1
        function T = solve_1(self)
        % Find the parameter T of a first-order transfer function
        % with k = 1000 and settling time of 0.4 seconds on impulse

            t_settle = 0.4;
            T = t_settle/3.91; % am ajuns la valoarea asta 
                                % in urma unui proces iterativ

            % graphic purposes
            
%             s = tf('s');
%             k = 10000;
%             H = k/(T * s + 1) ;
%             impulse(H);
             
            % T-ul nu trebuie modificat pentru a indeplini
            % in continuare conditia ca settle time sa fie la 0.4
            % intrucat k-ul influenteaza strict amplitudinea,
            % nu si timpul tranzitoriu
        end
        
        % 2
        function w = solve_2(self)
        % Find the w parameter of a second-order transfer function
        % with zeta = 0.2 and a peak time of 0.4 seconds on step

            z = 0.2;
            t_peak = 0.4;
            wd = pi/t_peak; % peak time formula
            w = wd/sqrt(1-z^2);
            
            % graphic purposes
            % s = tf('s');
            % H = w^2 / (s ^ 2 + 2 * z * w * s + w^2);
            % step(H);
            
        % pentru zeta = 0, amortizarea e 0 (caz ideal), asta
        % insemnand ca iesirea sistemului va oscila la nesfarsit
        % cu pulsatia naturala wn
        
        % pentru zeta foarte mare (aproximativ 1) vom avea
        % un raspuns neted, fara acel peak response obtinut
        % la overshoot
        end
        
        
        % 3
        function t_rise = solve_3(self, w, zetas)
        % Compute the rise times of a second-order function
        % with given w for each value zeta in zetas
            s = tf('s');
            t_rise = (2.230*zetas.^2 - 0.078*zetas + 1.12)/w;
            % aproximare cuadratica a t_rise
        end
        
        % 4
        function overshoots = solve_4(self, w, zetas)
        % Compute the overshoots of a second-order function
        % with given w for each value zeta in zetas
        % (don't convert to percents)
            s = tf('s');
            overshoots = exp(1).^(-pi*zetas./sqrt(1-zetas.^2));
            % formula din indrumar
        end
        
        % 5
        function t_stationary = solve_5(self)
        % Compute the time it takes for the Iron Man suit to stop.
        % Time and input are provided.
        
            s = tf('s');
            w = 12;
            z = 0.2;
            H = w^2 / (s^2 + 2*w*z*s + w^2);
            t = [0:0.1:10];         % Use this time vector
            u = [t <= 3] - [t > 3]; % Use this as input
            rsp_semnal = lsim(H, u, t);
            
            % graphic purposes
            % figure;
            % plot(t, rsp_semnal);
            
            % return with error of 0.1 when the Iron Man suit stop
            
            index = ((rsp_semnal < 0.1) & (rsp_semnal > -0.1));
            stationaryPoints = (find(index == 1));
            
            % t_stationary might return other states like
            % when the suit started (with rsp_semnal = 0) so return the
            % following state when the rsp_semnal is close to 0bgf
            
            t_stationary = t(stationaryPoints(2));
        end
    end
end