close all
clear
clc

%% Field parameters

% correlation length
Lc1 = 5;
Lc2 = 5;
% periodic correlation function
p1 = 100;
p2 = 100;
syms c1 c2
c1 = eval(vpasolve(0.5*p1*exp(-c1/2/Lc1^2)*besseli(0,c1/2/Lc1^2)-Lc1 == 0,c1));
c2 = eval(vpasolve(0.5*p2*exp(-c2/2/Lc2^2)*besseli(0,c2/2/Lc2^2)-Lc2 == 0,c2));
rho1 = @(tau) exp(-c1*sin(pi*tau/p1)^2/Lc1^2);
rho2 = @(tau) exp(-c2*sin(pi*tau/p2)^2/Lc2^2);
% non periodic correlation function
% rho1 = @(tau) exp(-pi*tau^2/4/Lc1^2);
% rho2 = @(tau) exp(-pi*tau^2/4/Lc2^2);
% tolerance
tol = 0.001;
% quantity name
name = 'Gc_5';
% mean value of the field
mean = 8e-4;
% coefficient of variance
CV = 0.3;
% number of realizations
num_realizations = 10;

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

%% preprocessor

prepro = PreProcessor(name,rho1,rho2,'cartesian',tol,mean,CV,num_realizations);

%% KL expansion

% Set up the covariance matrix and solve the expansion
[d,v] = KLexpansion(prepro,Xmesh,Ymesh);

for realization = 1:num_realizations
  
  % stochastic dimension = num of KL terms
  nu = length(d);
  
  %% visualize
  
  xi = randn(nu,1);
  G = v*(sqrt(d).*xi);
  field = GaussianToGamma(prepro,G);
  % Preprocess to match MOOSE syntax
  field = reshape(field,Ny,Nx);
  field = field';
  figure
  surf(Sx,Sy,field','EdgeColor','none');
  view(2);
  axis equal
  axis off
  colorbar
  
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