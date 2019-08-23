clear
clc

%% Field parameters

% Correlation length
Lcr = 50;
Lca = 4;
% tolerance
tol = 0.1;

%% Mesh

% Dimension of mesh
X1 = -50;
X2 = 50;
Y1 = -50;
Y2 = 50;

% Discretization of the mesh
Nx = 151;
Ny = 151;

% Generate mesh
Np = Nx*Ny;
Sx = linspace(X1,X2,Nx);
Sy = linspace(Y1,Y2,Ny);
[Xmesh,Ymesh] = meshgrid(Sx,Sy);

%% KL expansion

% Set up the covariance matrix and solve the expansion
[d,v] = KLexpansion_axisymmetric(Lcr,Lca,Xmesh,Ymesh,Np,tol);

% stochastic dimension = num of KL terms per field * num of fields
nu = length(d)*1;

%% visualize

xi = randn(nu,1);
G = v*(sqrt(d).*xi);
G = reshape(G,Ny,Nx);
surf(Sx,Sy,G,'EdgeColor','none');
axis equal
axis off
view(2);

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
