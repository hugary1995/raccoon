// outer radius
R = 6.5;

// height
H = 6.5;

// sphere radius of the heat source
r = 0.2;

// element characteristic length
E = 0.1;
e = 0.05;

Point(1) = {0,0,0,E};
Point(2) = {0,H-r,0,e};
Point(3) = {0,H,0,e};
Point(4) = {r,H,0,e};
Point(5) = {R,H,0,E};
Point(6) = {R,0,0,E};

Line(1) = {1,2};
Circle(2) = {2,3,4};
Line(3) = {4,5};
Line(4) = {5,6};
Line(5) = {6,1};
Line(6) = {4,3};
Line(7) = {3,2};

Line Loop(1) = {1,2,3,4,5};
Line Loop(2) = {2,6,7};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Recombine Surface{1,2};

Physical Surface("bulk") = {1};
Physical Surface("source") = {2};
Physical Line("left") = {1,7};
Physical Line("right") = {4};
Physical Line("top") = {3,6};
Physical Line("bottom") = {5};
