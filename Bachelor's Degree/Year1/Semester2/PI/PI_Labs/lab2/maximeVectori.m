function [max1, max2]= maximeVectori(v1, v2)
  if ((isvector(v1)==0) || (isvector(v2)== 0))
     disp("EROARE");
      max1 = 0;
      max2 = 0;
      return;
  end
  max1 = max(v1);
  max2 = max(v2);
end