% in urma aplicarii acestui filtru
% se obtine un semnal audio "lipsit" de zgomot
function[y, fs] = ii()
    
    [u, fs] = play_file("bass_test.wav", false);

    % fc = 1/(2*pi*r*c);
    % pentru fc = 100 => 200*pi = 1/(r*c)
    R = 1/(200*pi);
    C = 1;
    
   % num = s*R*C
   num = [R*C 0];
   % den = 1 + s*R*C
   den = [R*C 1];
   
   % H = s*R*C/(s*R*C + 1)
   H = tf(num, den);
    
   y = apply_filter(H, u, fs);
end