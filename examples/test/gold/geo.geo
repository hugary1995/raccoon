a = 1;
b = 1;

x = 0.6*a;
y = 0.55*b;

Point(1) = {0,0,0,0.8};
Point(2) = {0,b,0,0.6};
Point(3) = {a,b,0,0.6};
Point(4) = {a,0,0,0.5};

Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

Line Loop(1) = {1,2,3,4};

Plane Surface(1) = {1};
// Point{9} In Surface{1};

Physical Surface("all") = {1};
Physical Line("left") = {1};
Physical Line("right") = {3};
Physical Line("top") = {2};
Physical Line("bottom") = {4};
