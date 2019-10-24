function x = NR(residual,jacobian)

x = 0;
r = residual(x);

while r > 1e-10
  dr = jacobian(x);
  delta = -r/dr;
  x = x+delta;
  r = residual(x);
end