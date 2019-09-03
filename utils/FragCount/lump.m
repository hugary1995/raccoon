% filename = 'l_5';
filename = 'brick';
num_trials = 5;

result = [];
for i = 1:num_trials
  data = dlmread([filename,'_',num2str(i),'.csv'],' ');
  result = [result,data(:,2)];
end

lumped = [data(:,1),mean(result,2),std(result,0,2)];

errorbar(lumped(:,1),lumped(:,2),lumped(:,3))

dlmwrite([filename,'.csv'],lumped,' ');