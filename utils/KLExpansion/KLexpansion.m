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

parfor i = 1:Np
  for j = 1:Np
    if strcmp(coord,'cartesian')
      tau1 = abs(X(i)-X(j));
      tau2 = abs(Y(i)-Y(j));
    else
      tau1 = 0;
      tau2 = 0;
    end
    
    R(i,j) = rho_1(tau1)*rho_2(tau2);
  end
end

%% Find KL basis

disp('solving for eignevalues and associated KL basis...')

[V,D] = eig(R);
[d,ind] = sort(diag(D),'descend');
v = V(:,ind);

nu = 0;
err = 1;
R_trace = trace(R);
while err > prepro.tolerance
  nu = nu + 1;
  err = 1 - sum(d(1:nu))/R_trace;
end

d = d(1:nu);
v = v(:,1:nu);

fprintf('need %d eigenpairs to reach a tolerance of %.2f%%\n',nu,prepro.tolerance*100);

%% end

disp('------------------------------------------------')

end
