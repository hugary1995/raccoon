function [d,v] = KLexpansion(prepro,X,Y)
%%

disp('------------------------------------------------')

%% Correlation function

rho_1 = prepro.rho_1;
rho_2 = prepro.rho_2;

%% Correlation matrix

disp('setting up correlation matrix...')
Np = length(X(:));
R = eye(Np,Np);
coord = prepro.coordinate_system;

threshold = 0;

for i = 1:Np
  for j = 1:Np
    X1 = [X(i);Y(i)];
    X2 = [X(j);Y(j)];
    if strcmp(coord,'cartesian')
      tau = X1-X2;
      tau1 = abs(tau'*prepro.e1);
      tau2 = abs(tau'*prepro.e2);
    elseif strcmp(coord,'polar')
      center = [prepro.e1;prepro.e2];
      [tau1,tau2] = axisymmetricDistanceBetween(X1-center,X2-center);
    else
      tau1 = 0;
      tau2 = 0;
    end
    
    R(i,j) = single(rho_1(tau1)*rho_2(tau2));
  end
  progress = i/Np;
  if progress >= threshold && labindex == 1
    fprintf('progress: %.2f%%\n',progress*100);
    threshold = threshold+0.1;
  end
end

%% Find KL basis

disp('solving for eignevalues and associated KL basis...')

nu = 0;
R_trace = trace(R);

d = [];
v = [];
converged = false;

[V,D] = eig(R);
[dd,ind] = sort(diag(D),'descend');
vv = V(:,ind);
for i = 1:length(dd)
  d = [d;dd(i)];
  v = [v,vv(:,i)];
  nu = nu+1;
  err = 1-sum(d)/R_trace;
  if err <= prepro.tolerance
    clear R
    break
  end
end

% k = floor(Np/10);
%
% shift = 0;
%
% while ~converged
%   [V,D] = eigs(R,k,'largestreal');
%   [dd,ind] = sort(diag(D),'descend');
%   dd = dd+shift;
%   vv = V(:,ind);
%
%   for i = 1:length(dd)
%     d = [d;dd(i)];
%     v = [v,vv(:,i)];
%     nu = nu+1;
%     err = 1-sum(d)/R_trace;
%     if err <= prepro.tolerance
%       converged = true;
%       clear R
%       break
%     end
%   end
%
%   fprintf('%d eigenvalues have error = %.2f%%\n',nu,err*100);
%
%   if ~converged
%     R = R-V*D*V';
%     R = R-dd(1)*eye(Np);
%     shift = shift+dd(1);
%   end
%
% end

%% end

disp('------------------------------------------------')

end
