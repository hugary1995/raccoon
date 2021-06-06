close all
clc

%% Field parameters

% Correlation length
Lc1 = 10;
Lc2 = 10;
% tolerance
tol = 0.001;
% quantity name
name = 'Gc';
% Mean value of the field
average = 0.125;
% Coefficient of variance
CV = 0.3;
% Solving shape and scale parameters for a Gaussian distribution
A = 1/CV^2;
B = average*CV^2;

%% Mesh

% Dimension of mesh
X1 = 0;
X2 = 14;
Y1 = 0;
Y2 = 200;
r = 14;

% Discretization of the mesh
Nx = 101;
Ny = 101;

% Generate mesh
Np = Nx*Ny;
Sx = linspace(X1,X2,Nx);
Sy = linspace(Y1,Y2,Ny);
[Xmesh,Ymesh] = meshgrid(Sx,Sy);

%% KL expansion

% Set up the covariance matrix and solve the expansion
[d,v] = KLexpansion(r,Lc1,Lc2,Xmesh,Ymesh,Np,tol);

% stochastic dimension = num of KL terms per field * num of fields
nu = length(d)*1;

%% visualize

xi = randn(nu,1);
G = v*(sqrt(d).*xi);

gaussian = reshape(G,Ny,Nx);
gaussian = gaussian';
figure
h = surf(Sx,Sy,gaussian');
view(2);
axis equal
axis off
colorbar

field = gaminv(normcdf(G,0,1),A,B);
% Preprocess to match MOOSE syntax
field = reshape(field,Ny,Nx);
field = field';
figure
surf(Sx,Sy,field','EdgeColor','none');
view(2);
axis equal
axis off
colorbar

G = field(:);

%% write to txt

fprintf('writing field\n');

fileID = fopen([name,'.txt'],'wt');

% write X coordinates
fprintf(fileID,'AXIS X\n');
for i = 1:Nx-1
  fprintf(fileID,'%E ',Sx(i));
end
fprintf(fileID,'%E\n',Sx(Nx));

% write Y coordinates
fprintf(fileID,'AXIS Z\n');
for i = 1:Ny-1
  fprintf(fileID,'%E ',Sy(i));
end
fprintf(fileID,'%E\n',Sy(Ny));

% write normalized eigenvector
fprintf(fileID,'DATA\n');
for i = 1:Np-1
  fprintf(fileID,'%E ',field(i));
end
fprintf(fileID,'%E\n',field(Np));

fclose(fileID);