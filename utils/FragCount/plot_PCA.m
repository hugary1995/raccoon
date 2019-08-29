close all
clc

VV = [V;-V];

theta = [];

for i = 1:size(V,1)

  v = V(i,:);
  theta = [theta,atan(v(2)/v(1))+pi/2];
  theta = [theta,theta(end)+pi];

end

figure
plot(VV(:,1),VV(:,2),'.')
axis equal

figure
Theta = [theta-2*pi,theta,theta+2*pi];
[f,xi] = ksdensity(Theta,'Function','pdf','Support',[-2*pi,4*pi],'NumPoints',300);
plot(xi,f)

figure
polarplot(xi(100:200),f(100:200),'.-')

figure
polarhistogram(theta,40);