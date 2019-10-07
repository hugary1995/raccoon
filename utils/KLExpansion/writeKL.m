close all
clear
clc


%% parameters of the first field

% quantity name
name = 'Gc';
% correlation length
Lc1 = 10;
Lc2 = 10;
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
% mean value of the field
mean = 8e-4;
% coefficient of variance
CV = 0.3;
field1 = Field(name,rho1,rho2,'cartesian',tol,mean,CV);

%% parameters of the second field

% quantity name
name = 'psic';
% correlation length
Lc1 = 10;
Lc2 = 10;
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
% mean value of the field
mean = 3e-5;
% coefficient of variance
CV = 0.3;
field2 = Field(name,rho1,rho2,'cartesian',tol,mean,CV);

%% Mesh

% Dimension of mesh
X1 = 0;
X2 = 100;
Y1 = 0;
Y2 = 100;

% Discretization of the mesh
Nx = 151;
Ny = 151;

% Generate mesh
Np = Nx*Ny;
Sx = linspace(X1,X2,Nx);
Sy = linspace(Y1,Y2,Ny);
[Xmesh,Ymesh] = meshgrid(Sx,Sy);

%% sampling

num_realizations = 10;
rho = [0, 0.25, 0.5, 0.75, 1];
same_kernel = true;
sampler = Sampler(num_realizations,rho,field1,field2,Xmesh,Ymesh,same_kernel);
sampler.run();

