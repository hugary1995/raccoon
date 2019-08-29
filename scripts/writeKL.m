close all
clc

%% Field parameters

% Correlation length
Lc1 = 15;
Lc2 = 15;
% orientation
angle = 0;
e1 = [cos(angle);sin(angle)];
e2 = [-sin(angle);cos(angle)];
% Boolean for periodicity
XPeriodic = true;
YPeriodic = true;
% tolerance
tol = 0.001;
% quantity name
name = 'Gc_15';
% Mean value of the field
average = 8e-4;
% Coefficient of variance
CV = 0.3;
% Solving shape and scale parameters for a Gaussian distribution
A = 1/CV^2;
B = average*CV^2;
% number of realizations
num_realizations = 5;

%% Mesh

% Dimension of mesh
X1 = 0;
X2 = 100;
Y1 = 0;
Y2 = 100;

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
[d,v] = KLexpansion(e1,e2,Lc1,Lc2,Xmesh,Ymesh,Np,XPeriodic,YPeriodic,tol);

for realization = 1:num_realizations
  
  % stochastic dimension = num of KL terms
  nu = length(d);
  
  %% visualize
  
  xi = randn(nu,1);
  G = v*(sqrt(d).*xi);
  
  gaussian = reshape(G,Ny,Nx);
  gaussian = gaussian';
  % figure
  % h = surf(Sx,Sy,gaussian');
  % view(2);
  % axis equal
  % axis off
  % colorbar
  
  field = gaminv(normcdf(G,0,1),A,B);
  % Preprocess to match MOOSE syntax
  field = reshape(field,Ny,Nx);
  field = field';
  % figure
  % surf(Sx,Sy,field','EdgeColor','none');
  % view(2);
  % axis equal
  % axis off
  % colorbar
  
  G = field(:);
  
  %% write to txt
  
  fileID = fopen([name,'_',num2str(realization),'.txt'],'wt');
  
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
  
  % write normalized eigenvector
  fprintf(fileID,'DATA\n');
  for i = 1:Np-1
    fprintf(fileID,'%E ',field(i));
  end
  fprintf(fileID,'%E\n',field(Np));
  
  fclose(fileID);
  
end