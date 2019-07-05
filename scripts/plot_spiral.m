clear
close all
clc

theta0 = 10*pi;
theta1 = 26*pi;
theta = theta0:0.01*pi:theta1;
a = 0.06;
r0 = 100/exp(-a*theta0);
r = r0*exp(-a*theta);

x = r.*cos(theta);
y = r.*sin(theta);

plot(x,y)