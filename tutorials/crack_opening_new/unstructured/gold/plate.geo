e = 0.1;
ee = 0.012;

Point(1) = {0, 0, 0, e};
Point(2) = {0, 0.45, 0, ee};
Point(3) = {0, 0.55, 0, ee};
Point(4) = {0, 1, 0, e};
Point(5) = {1, 1, 0, e};
Point(6) = {1, 0.55, 0, ee};
Point(7) = {1, 0.45, 0, ee};
Point(8) = {1, 0, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 1};

Line(9) = {2, 7};
Line(10) = {3, 6};

Line Loop(1) = {1, 9, 7, 8};
Line Loop(2) = {2, 10, 6, -9};
Line Loop(3) = {3, 4, 5, -10};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Plane Surface(3) = {3};

Recombine Surface {1, 2, 3};

Physical Surface("all") = {1, 2, 3};
Physical Line("left") = {1, 2, 3};
Physical Line("top") = {4};
Physical Line("right") = {5, 6, 7};
Physical Line("bottom") = {8};
