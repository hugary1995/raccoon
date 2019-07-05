function [r,a] = axisymmetricDistanceBetween(p1,p2)

r1 = norm(p1);
r2 = norm(p2);
r = abs(r1-r2);

theta1 = getTheta(p1);
theta2 = getTheta(p2);
if theta1 > theta2
  thetaA = theta1;
  thetaB = theta2;
else
  thetaA = theta2;
  thetaB = theta1;
end
a = min([thetaA-thetaB,thetaB+2*pi-thetaA]);

end

function theta = getTheta(p)

x = p(1);
y = p(2);

if x == 0 && y == 0
  theta = 0;
  return
end

theta = atan(y/x);
if theta > 0 && y < 0
  theta = theta+pi;
elseif theta < 0 && y < 0
  theta = theta+2*pi;
elseif theta < 0 && y > 0
  theta = theta+pi;
elseif y == 0 && x < 0
  theta = pi;
end

end