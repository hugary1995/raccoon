function y = GaussianToGamma(prepro,x)

mean = prepro.mean;
CV = prepro.coef_variance;
A = 1/CV^2;
B = mean*CV^2;
y = gaminv(normcdf(x,0,1),A,B);