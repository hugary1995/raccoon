function d = spiralDistanceBetween(p1,p2,r0,a)

s1 = spiralTravelDistance(p1,r0,a);
s2 = spiralTravelDistance(p2,r0,a);

d = abs(s1-s2);

end

function s = spiralTravelDistance(p,r0,a)

x = p(1);
y = p(2);

if x == 0 && y == 0
  s = 0;
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

rp = norm(p);

r = r0*exp(-a*theta);
while r >= rp
  theta = theta+2*pi;
  r = r0*exp(-a*theta);
end

ap = -log(rp/r0)/theta;
s = r0/ap*exp(-ap*theta);

end