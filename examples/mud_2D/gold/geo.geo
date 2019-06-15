a = 100;
b = 100;
e = 0.75;

Point(1) = {0,0,0,e};
Point(2) = {0,b,0,e};
Point(3) = {a,b,0,e};
Point(4) = {a,0,0,e};
Point(5) = {a/2,b/2,0,e};
Point(6) = {a/2+a/10,b/2,0,e};

Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

Line Loop(1) = {1,2,3,4};

Plane Surface(1) = {1};
Point{5,6} In Surface{1};

Physical Surface("all") = {1};
Physical Line("left") = {1};
Physical Line("right") = {3};
Physical Line("top") = {2};
Physical Line("bottom") = {4};
Physical Point("center") = {5};
Physical Point("pin") = {6};
