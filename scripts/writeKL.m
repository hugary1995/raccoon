clear
clc

%% Field parameters

% Correlation length
Lcx = 4;
Lcy = 4;
% Boolean for periodicity
XPeriodic = true;
YPeriodic = true;
% tolerance
tol = 0.1;

%% Mesh

% Dimension of mesh
X1 = 0;
X2 = 20;
Y1 = 0;
Y2 = 20;

% Discretization of the mesh
Nx = 41;
Ny = 41;

% Generate mesh
Np = Nx*Ny;
Sx = linspace(X1,X2,Nx);
Sy = linspace(Y1,Y2,Ny);
[Xmesh,Ymesh] = meshgrid(Sx,Sy);

%% KL expansion

% Set up the covariance matrix and solve the expansion
[d,v] = KLexpansion(Lcx,Lcy,Xmesh,Ymesh,Np,XPeriodic,YPeriodic,tol);

% stochastic dimension = num of KL terms per field * num of fields
nu = length(d)*1;

%% write to txt

fileID = fopen('kl_info.txt','wt');

% write X coordinates
fprintf(fileID,'AXIS X\n');
for i = 1:Nx-1
    fprintf(fileID,'%E ',Sx(i));
end
fprintf(fileID,'%E\n',Sx(Nx));

% write Y coordinates
fprintf(fileID,'AXIS Y\n');
for i = 1:Ny-1
    fprintf(fileID,'%E ',Sy(i));
end
fprintf(fileID,'%E\n',Sy(Ny));

for n = 1:nu
    
    fprintf('writing field %d into kl_info.txt\n',n);
    
    % normalize eigenvector, v_n_normalized = sqrt(d_n)*v_n
    field = v(:,n) * sqrt(d(n));
        
    % Preprocess eigenvector to match MOOSE syntax
    field = reshape(field,Ny,Nx);
    field = field';
    
    % write normalized eigenvector
    fprintf(fileID,'DATA\n');
    for i = 1:Np-1
        fprintf(fileID,'%E ',field(i));
    end
    fprintf(fileID,'%E\n',field(Np));
    
end

fclose(fileID);
