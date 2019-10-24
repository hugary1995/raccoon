function L = dstruveL0(x)

L = 0;
T0 = 2/pi;
k = 0;
Tk = T0;

L = L+Tk;

while Tk > 1e-10*T0
  Tk = Tk*4*x^2*(k+1)*(k+2)/(2*k+1)/(2*k+2)/(2*k+3)/(2*k+4);
  L = L+Tk;
  k = k+1;
end