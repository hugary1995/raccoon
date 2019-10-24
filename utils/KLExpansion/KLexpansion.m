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
    if strcmp(coord,'cartesian')
      tau1 = abs(X(i)-X(j));
      tau2 = abs(Y(i)-Y(j));
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
err = 1;
d = [];
v = [];
converged = false;

k = floor(Np/10);

while ~converged
  [V,D] = eigs(R,k,'largestreal');
  [dd,ind] = sort(diag(D),'descend');
  vv = V(:,ind);
  
  for i = 1:length(dd)
    d = [d;dd(i)];
    v = [v,vv(:,i)];
    nu = nu+1;
    err = 1-sum(d)/R_trace;
    if err <= prepro.tolerance
      converged = true;
      clear R
      break
    end
  end
  
  fprintf('%d eigenvalues have error = %.2f%%\n',nu,err*100);
  
  if ~converged
    R = R-V*D*V';
  end
  
end

%% end

disp('------------------------------------------------')

end
