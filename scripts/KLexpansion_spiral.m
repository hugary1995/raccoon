function [d,v] = KLexpansion_spiral(Lc,r0,a,X,Y,Np,tol)
%%

disp('------------------------------------------------')

%% Correlation function

C = @(tau, Lc) exp(-pi*tau^2/4/Lc^2);

%% Correlation matrix

disp('setting up correlation matrix...')
R = eye(Np,Np);
count = 0;
progress = 0;
p_step = 0.01;
nstars = 0;
nspaces = 0;
fprintf('     ');

parfor i = 1:Np
  for j = 1:Np
    p1 = [X(i);Y(i)];
    p2 = [X(j);Y(j)];
    tau = spiralDistanceBetween(p1,p2,r0,a);
    R(i,j) = C(tau,Lc);
%     count = count + 1;
  end
  % print progress bar, doesn't do any real work
%   if 2*count/Np/(Np-1) >= (progress + p_step)
%     fprintf(repmat('\b',1,nstars+nspaces+5));
%     progress = progress + p_step;
%     nstars = round(50*progress);
%     nspaces = round(50*(1-progress));
%     fprintf('||');
%     fprintf(repmat('>',1,nstars));
%     fprintf(repmat(' ',1,nspaces));
%     fprintf('||\n');
%     pause(.01);
%   end
end

% for i = 1:Np
%   for j = 1:i-1
%     R(i,j) = R(j,i);
%   end
% end

%% Find KL basis

disp('solving for eignevalues and associated KL basis...')

[V,D] = eigs(R,50);
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
