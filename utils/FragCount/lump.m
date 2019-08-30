l = 15;
num_trials = 5;

result = [];
for i = 1:num_trials
  data = dlmread(['l_',num2str(l),'_',num2str(i),'.csv'],' ');
  result = [result,data(:,2)];
end

lumped = [data(:,1),mean(result,2),std(result,0,2)];

errorbar(lumped(:,1),lumped(:,2),lumped(:,3))

dlmwrite(['l_',num2str(l),'.csv'],lumped,' ');