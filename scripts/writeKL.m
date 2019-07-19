clear
clc

%% Field parameters

% Correlation length
Lc1 = 0.5;
Lc2 = 0.5;
% orientation
angle = 0;
e1 = [cos(angle);sin(angle)];
e2 = [-sin(angle);cos(angle)];
% Boolean for periodicity
XPeriodic = false;
YPeriodic = false;
% tolerance
tol = 0.1;
% quantity name
name = 'Gc';
% Mean value of the field
average = 2.188e-2;
% Coefficient of variance
CV = 0.3;
% Solving shape and scale parameters for a Gaussian distribution
A = 1/CV^2;
B = average*CV^2;

%% Mesh

% Dimension of mesh
X1 = 0;
X2 = 6.5;
Y1 = 0;
Y2 = 6.5;

% Discretization of the mesh
Nx = 66;
Ny = 66;

% Generate mesh
Np = Nx*Ny;
Sx = linspace(X1,X2,Nx);
Sy = linspace(Y1,Y2,Ny);
[Xmesh,Ymesh] = meshgrid(Sx,Sy);

%% KL expansion

% Set up the covariance matrix and solve the expansion
[d,v] = KLexpansion(e1,e2,Lc1,Lc2,Xmesh,Ymesh,Np,XPeriodic,YPeriodic,tol);

% stochastic dimension = num of KL terms per field * num of fields
nu = length(d)*1;

%% visualize

xi = randn(nu,1);
G = v*(sqrt(d).*xi);
field = gaminv(normcdf(G,0,1),A,B);
G = reshape(field,Ny,Nx);
surf(Sx,Sy,G);
view(2);

%% write to txt

fileID = fopen([name,'.txt'],'wt');

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

fprintf('writing field\n');

% Preprocess eigenvector to match MOOSE syntax
field = reshape(field,Ny,Nx);
field = field';

% write normalized eigenvector
fprintf(fileID,'DATA\n');
for i = 1:Np-1
  fprintf(fileID,'%E ',field(i));
end
fprintf(fileID,'%E\n',field(Np));

fclose(fileID);
