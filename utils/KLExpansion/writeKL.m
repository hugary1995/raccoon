close all
clear
clc

%% parameters of the first field

% quantity name
name = 'Gc_L_8';

% coordinate system
theta = 0;
e1 = [cos(theta);sin(theta)];
e2 = [-sin(theta);cos(theta)];
% e1 = 50;
% e2 = 50;

% correlation length
Lc1 = 8;
Lc2 = 8;

% periodic correlation function
% p1 = 100;
% p2 = 100;

% p-periodic squared exponential
% syms c1
% c1 = eval(vpasolve(0.5*p1*exp(-c1/2/Lc1^2)*besseli(0,c1/2/Lc1^2)-Lc1 == 0,c1));
% rho1 = @(tau) exp(-c1*sin(pi*tau/p1).^2/Lc1^2);

% syms c2
% c2 = eval(vpasolve(0.5*p2*exp(-c2/2/Lc2^2)*besseli(0,c2/2/Lc2^2)-Lc2 == 0,c2));
% rho2 = @(tau) exp(-c2*sin(pi*tau/p2).^2/Lc2^2);

% p-periodic exponential
% r1 = @(c1) 0.5*p1*(besseli(0,c1/Lc1)-struveL0(c1/Lc1))-Lc1;
% dr1 = @(c1) 0.5*p1/Lc1*(besseli(1,c1/Lc1)-dstruveL0(c1/Lc1));
% c1 = NR(r1,dr1);
% rho1 = @(tau) exp(-c1*abs(sin(pi*tau/p1))/Lc1);

% r2 = @(c2) 0.5*p2*(besseli(0,c2/Lc2)-struveL0(c2/Lc2))-Lc2;
% dr2 = @(c2) 0.5*p2/Lc2*(besseli(1,c2/Lc2)-dstruveL0(c2/Lc2));
% c2  = NR(r2,dr2);
% rho2 = @(tau) exp(-c2*abs(sin(pi*tau/p2))/Lc2);

% non periodic squared exponential
rho1 = @(tau) exp(-pi*tau^2/4/Lc1^2);
rho2 = @(tau) exp(-pi*tau^2/4/Lc2^2);

% non periodic exponential
% rho1 = @(tau) exp(-tau/Lc1);
% rho2 = @(tau) exp(-tau/Lc2);

% tolerance
tol = 1e-2;

% mean value of the field
mean = 8e-4;

% coefficient of variance
CV = 0.03;

field1 = Field(name,rho1,rho2,'cartesian',e1,e2,tol,mean,CV);

%% parameters of the second field

% quantity name
name = 'psic_L_8';

% coordinate system
theta = 0;
e1 = [cos(theta);sin(theta)];
e2 = [-sin(theta);cos(theta)];
% e1 = 50;
% e2 = 50;

% correlation length
Lc1 = 8;
Lc2 = 8;

% periodic correlation function
% p1 = 100;
% p2 = 100;

% p-periodic squared exponential
% syms c1
% c1 = eval(vpasolve(0.5*p1*exp(-c1/2/Lc1^2)*besseli(0,c1/2/Lc1^2)-Lc1 == 0,c1));
% rho1 = @(tau) exp(-c1*sin(pi*tau/p1).^2/Lc1^2);

% syms c2
% c2 = eval(vpasolve(0.5*p2*exp(-c2/2/Lc2^2)*besseli(0,c2/2/Lc2^2)-Lc2 == 0,c2));
% rho2 = @(tau) exp(-c2*sin(pi*tau/p2).^2/Lc2^2);

% p-periodic exponential
% r1 = @(c1) 0.5*p1*(besseli(0,c1/Lc1)-struveL0(c1/Lc1))-Lc1;
% dr1 = @(c1) 0.5*p1/Lc1*(besseli(1,c1/Lc1)-dstruveL0(c1/Lc1));
% c1 = NR(r1,dr1);
% rho1 = @(tau) exp(-c1*abs(sin(pi*tau/p1))/Lc1);

% r2 = @(c2) 0.5*p2*(besseli(0,c2/Lc2)-struveL0(c2/Lc2))-Lc2;
% dr2 = @(c2) 0.5*p2/Lc2*(besseli(1,c2/Lc2)-dstruveL0(c2/Lc2));
% c2  = NR(r2,dr2);
% rho2 = @(tau) exp(-c2*abs(sin(pi*tau/p2))/Lc2);

% non periodic squared exponential
rho1 = @(tau) exp(-pi*tau^2/4/Lc1^2);
rho2 = @(tau) exp(-pi*tau^2/4/Lc2^2);

% non periodic exponential
% rho1 = @(tau) exp(-tau/Lc1);
% rho2 = @(tau) exp(-tau/Lc2);

% tolerance
tol = 1e-2;

% mean value of the field
mean = 3e-5;

% coefficient of variance
CV = 0.03;

field2 = Field(name,rho1,rho2,'cartesian',e1,e2,tol,mean,CV);

%% Mesh

% Dimension of mesh
X1 = -112.5;
X2 = 112.5;
Y1 = -112.5;
Y2 = 112.5;

% Discretization of the mesh
Nx = 101;
Ny = 101;

% Generate mesh
Np = Nx*Ny;
Sx = linspace(X1,X2,Nx);
Sy = linspace(Y1,Y2,Ny);
[Xmesh,Ymesh] = meshgrid(Sx,Sy);

%% sampling mode
% default is random
% provide a positive seed to generate reproducible samples
% field1.seed = string2hash('field_1_seed_h');
% field2.seed = string2hash('field_2_seed_h');

%% sampling

num_realizations = 1;
rho = [0];
same_kernel = true;
sampler = Sampler(num_realizations,rho,field1,field2,Xmesh,Ymesh,same_kernel);
sampler.run();

