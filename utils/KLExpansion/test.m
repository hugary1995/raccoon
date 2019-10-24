L = [];
X = -10:0.1:10;
for x = X
  L = [L, struveL0(x)];
end
plot(X,L);