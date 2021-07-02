x22=[1:0.01:8];
err = x22*0.02;
y22 = 5*(1 - exp(-2*x22))+err;
cftool