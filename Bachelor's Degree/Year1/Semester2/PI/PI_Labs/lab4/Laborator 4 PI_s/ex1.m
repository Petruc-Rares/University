function [] = ex1(data)

    n = size(data, 2);
    disp('mvl = ');
    disp(mean(data));
    
    sortedData = sort(data);
    disp('me = ');
    if (mod(n, 2) ~= 0)
       
        disp(sortedData(n/2 + 1));
       
    else 
        
        disp((sortedData(n/2) + sortedData(n/2+1))/2);
        
    end
    
    niuData = data - mean(data);
    niuData = niuData.^2;
    disp('niu = ');
    disp(sqrt(1/n*mean(niuData)));
    
    disp('sigl = ');
    disp(sqrt(1/(n-1)*mean(niuData)));
    
    disp('teta = ');
    disp(1/n*mean(abs(data-mean(data))));
end