function [w] = learn(X, y, lr, epochs)

    Nc = size(X)(2);
    Nr = size(X)(1);
 
    batch_size = 64;
    
    % valori random pentru w din intervalul specificat
    
    w = -0.1 + 0.2*rand(Nc+1, 1);
    
    X = X - mean(X);
    
    X = X./std(X);
    
    b = zeros(Nr, 1);
    b(1:Nr) = 1;
    
    X = [X, b];
    
    for epoch = 1:epochs
    
        randlines = ceil(rand(batch_size, 1) * Nr);
        
        Xbatch = X(randlines, :);
        
        ybatch = y(randlines);
        
        S = sum((Xbatch*w - ybatch).*Xbatch);
        S = lr/Nr*S;
        w = w - S';
      
    endfor

endfunction
