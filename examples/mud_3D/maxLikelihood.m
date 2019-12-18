close all
clear
clc

load('exp');
exp_sizes = sizes;

load('sim');
sim_sizes = sizes;

A = pi*112.5^2;
Ls = {'3','3.5','4','4.5','5','5.5','6','6.5','7','7.5','8','8.5'};
for i = 1:length(Ls)
  f = ksdensity(sim_sizes{i},exp_sizes,'Bandwidth',0.3,...
    'Support',[0,A]);
  L = sum(log(f));
  fprintf('L = %s, likelihood = %.3f\n',Ls{i},L);
%   figure('Position',[0,0,250,250])
%   plot(x,f,'k','LineWidth',3)
%   xlim([0,1000])
%   xlabel('fragment size (mm^2)')
%   ylabel('PDF')
%   print(['visualize/pdf_',Ls{i},'.png'],'-dpng');
end

[f_exp,x_exp] = ksdensity(exp_sizes,0:A/10000:A,'Bandwidth',0.3,...
    'BoundaryCorrection','log','Support',[0,A]);
[f_sim,x_sim] = ksdensity(sim_sizes{3},0:A/10000:A,'Bandwidth',0.3,...
    'BoundaryCorrection','log','Support',[0,A]);
  
plot(x_exp,f_exp);
hold on
plot(x_sim,f_sim);
hold off
xlim([0,1000])
legend('experiment','L = 4')