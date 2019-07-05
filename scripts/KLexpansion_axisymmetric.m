function [d,v] = KLexpansion_axisymmetric(Lcr,Lca,X,Y,Np,tol)
%%

disp('------------------------------------------------')

%% Correlation function

Cr = @(tau, Lc) exp(-pi*tau^2/4/Lc^2);
Ca = @(tau, Lc) exp(-pi*tau^2/4/Lc^2);

%% Correlation matrix

disp('setting up correlation matrix...')
R = eye(Np,Np);

parfor i = 1:Np
  for j = 1:Np
    p1 = [X(i);Y(i)];
    p2 = [X(j);Y(j)];
    [r,a] = axisymmetricDistanceBetween(p1,p2);
    R(i,j) = Cr(r,Lcr)*Ca(a*(norm(p1)+norm(p2))/2,Lca);
  end
end

%% Find KL basis

disp('solving for eignevalues and associated KL basis...')

[V,D] = eigs(R,100);
[d,ind] = sort(diag(D),'descend');
v = V(:,ind);

nu = 0;
err = 1;
R_trace = trace(R);
while err > tol
  nu = nu + 1;
  err = 1 - sum(d(1:nu))/R_trace;
end

d = d(1:nu);
v = v(:,1:nu);

fprintf('need %d eigenpairs to reach a tolerance of %.2f%%\n',nu,tol*100);

%% end

disp('------------------------------------------------')

end
