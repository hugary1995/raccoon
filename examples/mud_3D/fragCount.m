function sizes = fragCount(filename,h,r,dc,A)

all = csvread(filename,1,0);

% trim M
all = all(:,[4,5,6,1]);

% extract values at the surface z = h
all = all(all(:,3) == h,[1,2,4])';

% figure
% scatter(all(1,:),all(2,:),5,all(3,:))
% xlim([-112.5,112.5]);
% ylim([-112.5,112.5]);

good = all(:,all(3,:) < dc);
bad = all(:,all(3,:) >= dc);

% area per point
a = A/size(good,2);

% initialize
sizes = [];
queue = [];
% figure

% while queue is not empty
while ~isempty(good)
  [good,queue] = moveFromTo(good,queue,1);
  cluster = [];
  while ~isempty(queue)
    n = neighbors(good,queue(:,1),r,dc);
    [good,queue] = moveFromTo(good,queue,n);
    [queue,cluster] = moveFromTo(queue,cluster,1);
  end
  
  % cluster size
  Ac = a*size(cluster,2);
  sizes = [sizes,Ac];
  fprintf('yet to process %d/%d points\n',size(good,2),size(all,2));
  
%   scatter(cluster(1,:),cluster(2,:),5,cluster(3,:))
%   hold on
%   drawnow
end

end

function d = distance(all,p)

c = size(all,2);
d = zeros(1,c);
all = all(1:2,:)-p(1:2);
for i = 1:c
  d(i) = norm(all(:,i));
end

end

function index = neighbors(all,p,r,dc)

index = distance(all,p) < r & all(3,:) < dc;

end

function [from,to] = moveFromTo(from,to,index)

to = [to,from(:,index)];
from(:,index) = [];

end