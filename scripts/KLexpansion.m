function [d,v] = KLexpansion(type,Lc,X,Y,Np,XPeriodic,YPeriodic,tol)
%%

disp('------------------------------------------------')

%% Mesh

% Extract domain size from mesh
Lx = max(X(:))-min(X(:));
Ly = max(Y(:))-min(Y(:));

%% Correlation function

if type == 0
    C = @(tau, Lc) exp(-abs(tau)/Lc);
elseif type == 1
    Lc = sqrt(2)/sqrt(pi)*Lc;
    C = @(tau, Lc) exp(-tau^2/2/Lc^2);
elseif type == 2
    Lc = sqrt(3)/2*Lc;
    C = @(tau, Lc) (1+sqrt(3)*abs(tau)/Lc)*exp(-sqrt(3)*abs(tau)/Lc);
elseif type == 3
    Lc = 3*sqrt(5)/8*Lc;
    C = @(tau, Lc) (1+sqrt(5)*abs(tau)/Lc+5*tau^2/3/Lc^2)*exp(-sqrt(5)*abs(tau)/Lc);
else
    disp('type error, should be 1, 2, or 3')
    exit;
end

%% Correlation matrix

disp('setting up correlation matrix...')
R = eye(Np,Np);
count = 0;
progress = 0;
p_step = 0.01;
nstars = 0;
nspaces = 0;
fprintf('     ');

for i = 1:Np
    for j = (i+1):Np
        correlation = zeros(1,5);
        correlation(1) = abs(C(X(i)-X(j),Lc)*C(Y(i)-Y(j),Lc));
        % If the field is periodic in X, we create two phantom meshes on
        % left and right and choose the largest correlation
        if XPeriodic
            correlation(2) = abs(C(X(i)-X(j)-Lx,Lc)*C(Y(i)-Y(j),Lc));
            correlation(3) = abs(C(X(i)-X(j)+Lx,Lc)*C(Y(i)-Y(j),Lc));
        end
        if YPeriodic
            correlation(4) = abs(C(X(i)-X(j),Lc)*C(Y(i)-Y(j)-Ly,Lc));
            correlation(5) = abs(C(X(i)-X(j),Lc)*C(Y(i)-Y(j)+Ly,Lc));
        end
        R(i,j) = max(correlation);
        count = count + 1;
    end
    % print progress bar, doesn't do any real work
    if 2*count/Np/(Np-1) >= (progress + p_step)
        fprintf(repmat('\b',1,nstars+nspaces+5));
        progress = progress + p_step;
        nstars = round(50*progress);
        nspaces = round(50*(1-progress));
        fprintf('||');
        fprintf(repmat('>',1,nstars));
        fprintf(repmat(' ',1,nspaces));
        fprintf('||\n');
        pause(.01);
    end
end
R = R + R' - diag(diag(R));

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
