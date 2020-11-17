// element sizes
a = 50;
e = 2;

Point(0) = {0, 0, 0, e};
Point(1) = {a, 0, 0, e};
Point(2) = {0, a, 0, e};
Point(3) = {-a, 0, 0, e};
Point(4) = {0, -a, 0, e};

Circle(1) = {1, 0, 2};
Circle(2) = {2, 0, 3};
Circle(3) = {3, 0, 4};
Circle(4) = {4, 0, 1};

Line Loop(1) = {1, 2, 3, 4};

Plane Surface(1) = {1};

Physical Surface("all") = {1};
