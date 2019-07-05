function [d,v] = KLexpansion(e1,e2,Lc1,Lc2,X,Y,Np,XPeriodic,YPeriodic,tol)
%%

disp('------------------------------------------------')

%% Mesh

% Extract domain size from mesh
Lx = max(X(:))-min(X(:));
Ly = max(Y(:))-min(Y(:));

%% Correlation function

C = @(tau, Lc) exp(-pi*tau^2/4/Lc^2);

%% Correlation matrix

disp('setting up correlation matrix...')
R = eye(Np,Np);

parfor i = 1:Np
  for j = 1:Np
    correlation = zeros(1,5);
    correlation(1) = abs(C(e1'*[X(i)-X(j);Y(i)-Y(j)],Lc1)*C(e2'*[X(i)-X(j);Y(i)-Y(j)],Lc2));
    % If the field is periodic in X, we create two phantom meshes on
    % left and right and choose the largest correlation
    if XPeriodic
      correlation(2) = abs(C(e1'*[X(i)-X(j)-Lx;Y(i)-Y(j)],Lc1)*C(e2'*[X(i)-X(j)-Lx;Y(i)-Y(j)],Lc2));
      correlation(3) = abs(C(e1'*[X(i)-X(j)+Lx;Y(i)-Y(j)],Lc1)*C(e2'*[X(i)-X(j)+Lx;Y(i)-Y(j)],Lc2));
    end
    if YPeriodic
      correlation(4) = abs(C(e1'*[X(i)-X(j);Y(i)-Y(j)-Ly],Lc1)*C(e2'*[X(i)-X(j);Y(i)-Y(j)-Ly],Lc2));
      correlation(5) = abs(C(e1'*[X(i)-X(j);Y(i)-Y(j)+Ly],Lc1)*C(e2'*[X(i)-X(j);Y(i)-Y(j)+Ly],Lc2));
    end
    R(i,j) = max(correlation);
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
