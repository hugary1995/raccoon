w = 1.5;
h = 0.5;

E = 0.1;
L = 0.05;
n = 5;
e = L/n;

Point(1) = {0, 0, 0, e};
Point(2) = {0, 2*L, 0, e};
Point(3) = {0, h, 0, E};
Point(4) = {w, h, 0, E};
Point(5) = {w, 2*L, 0, e};
Point(6) = {w, 0, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 1};
Line(7) = {2, 5};

Line Loop(1) = {1, 7, 5, 6};
Line Loop(2) = {2, 3, 4, -7};

Plane Surface(1) = {1};
Plane Surface(2) = {2};

Transfinite Line {1, 5} = 2*L/e+1 Using Progression 1;
Transfinite Line {6, 7} = w/e+1 Using Progression 1;
Transfinite Surface {1};
Recombine Surface {1, 2};

Physical Surface("all") = {1, 2};
Physical Line("top") = {3};
Physical Line("bottom") = {6};
Physical Line("right") = {4, 5};
Physical Line("left") = {1, 2};
