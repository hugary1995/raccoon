function expCount(filename)

A = imread(filename);
A = rgb2gray(A);
A = double(A);

% figure
% imshow(A,[])

sizes = [];
queue = [];
figure
count = 0;
while true
  count = count+1;
  % move one to queue
  [i,j] = find(A==255);
  if length(i) < 1
    break
  end
  [A,queue] = movePixelFromImageToQueue(A,queue,i(1),j(1));
  cluster = [];
  while ~isempty(queue)
    [A,queue] = moveNeighborsFromImageToQueue(A,queue,queue(1,1),queue(1,2));
    [queue,cluster] = movePixelFromQueueToQueue(queue,cluster,1);
  end
  fprintf('cluster size: %d, queue size: %d\n',size(cluster,1),size(queue,1));
  scatter(cluster(:,1),cluster(:,2),1,count*ones(size(cluster,1),1));
  sizes = [sizes,size(cluster,1)];
  drawnow
  hold on
end
hold off

Apixel = pi*112.5^2/sum(sizes);
sizes = sizes*Apixel;
save('exp','sizes');

end

function [from,to] = movePixelFromQueueToQueue(from,to,i)

to = [to;from(i,:)];
from(i,:) = [];

end

function [from,to] = movePixelFromImageToQueue(from,to,i,j)

from(i,j) = -1;
to = [to;i,j];

end

function [from,to] = moveNeighborsFromImageToQueue(from,to,i,j)

n = size(from,1);
if j-1 >= 1 && from(i,j-1) == 255
  [from,to] = movePixelFromImageToQueue(from,to,i,j-1);
end

if j+1 <= n && from(i,j+1) == 255
  [from,to] = movePixelFromImageToQueue(from,to,i,j+1);
end

if i-1 >= 1 && from(i-1,j) == 255
  [from,to] = movePixelFromImageToQueue(from,to,i-1,j);
end

if i+1 <= n && from(i+1,j) == 255
  [from,to] = movePixelFromImageToQueue(from,to,i+1,j);
end

end