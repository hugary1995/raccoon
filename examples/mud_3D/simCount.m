close all
clear
clc

Ls = {'3','3.5','4','4.5','5','5.5','6','6.5','7','7.5','8','8.5'};

A = pi*112.5^2;

sizes = {};
for i = 1:length(Ls)
  fprintf('counting L = %s\n',Ls{i});
  sizes{i} = fragCount(['L_',Ls{i},'/',Ls{i},'.csv'],4,1.5,0.75,A);
end

save('sim','sizes');