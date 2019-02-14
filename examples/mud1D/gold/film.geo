a = 20;
h = 1;
e = 0.08;

Point(1) = {0,0,0,e};
Point(2) = {0,h,0,e};
Point(3) = {a,h,0,e};
Point(4) = {a,0,0,e};

Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

Line Loop(1) = {1,2,3,4};

Plane Surface(1) = {1};

Physical Surface("film") = {1};
Physical Line("bottom") = {4};
Physical Line("left") = {1};
Physical Line("right") = {3};
Physical Line("top") = {2};
