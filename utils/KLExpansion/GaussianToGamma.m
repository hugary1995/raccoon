function y = GaussianToGamma(mean,CV,x)

A = 1/CV^2;
B = mean*CV^2;
y = gaminv(normcdf(x,0,1),A,B);