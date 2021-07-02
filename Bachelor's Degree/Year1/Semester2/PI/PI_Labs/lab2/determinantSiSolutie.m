function [x, detA] = determinantSiSolutie(A, b)
% SISTECLIN rezolva sistemul de ecuatii liniare A*x=b si
% returneaza vectorul solutie x, precum si determinantul matricei A

detA=det(A);
x=A\b;
end