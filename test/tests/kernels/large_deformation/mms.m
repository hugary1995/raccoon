% MMS

clear
clc

syms x
syms y

nu = 0.3;
% youngs modulus is defined such that the factor before stiffness matrix is
% 1, i.e., for plane strain E = (1+nu)*(1-2*nu);

X = 0.1*(sin(0.5*pi*x) + y^3);
Y = 0.1*(sin(0.5*pi*y) + x^3);

A = [diff(X,x),diff(X,y);
    diff(Y,x),diff(Y,y)];
F = eye(2)+A

E = (transpose(F)*F-eye(2))/2;
E = simplify(E)

E_ = [E(1,1);E(2,2);E(1,2)];
D_ = [1-nu,nu,0;nu,1-nu,0;0,0,1-2*nu];
S_ = D_*E_;

S = [S_(1),S_(3);
    S_(3),S_(2)];
S = simplify(S)

P = simplify(F*S)

b1 = -diff(P(1,1),x)-diff(P(1,2),y);
b1 = simplify(b1)

b2 = -diff(P(2,1),x)-diff(P(2,2),y);
b2 = simplify(b2)