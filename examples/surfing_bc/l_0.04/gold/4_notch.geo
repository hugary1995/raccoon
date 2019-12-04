w = 1.5;
h = 0.5;
a = 0.2;

E = 0.1;
L = 0.04;
n = 8;
e = L/n;

Point(1) = {0, 0, 0, e};
Point(2) = {0, 2*L, 0, e};
Point(3) = {0, h, 0, E};
Point(4) = {w, h, 0, E};
Point(5) = {w, 2*L, 0, e};
Point(6) = {w, 0, 0, e};
Point(7) = {a, 0, 0, e};
Point(8) = {a, 2*L, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 1};
Line(8) = {2, 8};
Line(9) = {8, 5};
Line(10) = {8, 7};

Line Loop(1) = {1, 8, 10, 7};
Line Loop(2) = {-10, 9, 5, 6};
Line Loop(3) = {2, 3, 4, -9, -8};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Plane Surface(3) = {3};

Transfinite Line {1, 5, 10} = 2*L/e+1 Using Progression 1;
Transfinite Line {6, 9} = (w-a)/e+1 Using Progression 1;
Transfinite Line {7, 8} = a/e+1 Using Progression 1;
Transfinite Surface {1, 2};
Recombine Surface {1, 2, 3};

Physical Surface("all") = {1, 2, 3};
Physical Line("top") = {3};
Physical Line("bottom") = {6};
Physical Line("right") = {4, 5};
Physical Line("left") = {1, 2};
