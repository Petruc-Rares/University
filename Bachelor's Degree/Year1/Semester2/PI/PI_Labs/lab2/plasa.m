function [] = plasa()
    x = [-10:1:10];
    y = [0:3:30];
    [X, Y] = meshgrid(x, y);
    Z = Y.*sin(X);
    mesh(X, Y, Z);
end

