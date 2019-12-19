W = 4;
H = 4;

w = 0.8;
h = 0.4;

E = 0.25;
e = 0.008;

Point(1) = {0, 0, 0, E};
Point(2) = {0, H, 0, E};
Point(3) = {W, H, 0, E};
Point(4) = {W, 0, 0, E};

Point(5) = {W/2-w/2, H/2-h/2, 0, e};
Point(6) = {W/2-w/2, H/2+h/2, 0, e};
Point(7) = {W/2+w/2, H/2+h/2, 0, e};
Point(8) = {W/2+w/2, H/2-h/2, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 5};

Line Loop(1) = {1, 2, 3, 4};
Line Loop(2) = {5, 6, 7, 8};

Plane Surface(1) = {1, 2};
Plane Surface(2) = {2};

Transfinite Line {5, 7} = h/e+1 Using Progression 1;
Transfinite Line {6, 8} = w/e+1 Using Progression 1;
Transfinite Surface {2};
Recombine Surface {1, 2};

Physical Surface("all") = {1, 2};
Physical Line("top") = {2};
Physical Line("bottom") = {4};
Physical Line("right") = {3};
Physical Line("left") = {1};
