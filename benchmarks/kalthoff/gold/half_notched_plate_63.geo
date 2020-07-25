// the projectile is assumed to be 63 degree
angle = (90-63)*Pi/180;

E = 5;
e = 0.2;
eps = 0.001;

Point(1) = {0, 0, 0, E};
Point(2) = {0, 25-eps, 0, E};
Point(3) = {50, 25, 0, e};
Point(4) = {0, 25+eps, 0, E};
Point(5) = {0, 100, 0, E};
Point(6) = {50+75*Tan(angle), 100, 0, e};
Point(7) = {100, 100, 0, E};
Point(8) = {100, 0, 0, E};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 1};

Line(9) = {3, 6};

Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};

Plane Surface(1) = {1};
Line{9} In Surface {1};

Physical Surface("all") = {1};
Physical Line("load") = {1};
Physical Line("bottom") = {8};
