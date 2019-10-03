function ave = plotRollingAverage(G)

ave = cumsum(G)'./(1:length(G));

plot(ave)