w = 100;
h = 100;

xc = 50;
yc = 60;
r = 20;

E = 2.5;
e = 0.6;

Point(1) = {0, 0, 0, E};
Point(2) = {0, 80, 0, e};
Point(3) = {0, h, 0, E};
Point(4) = {w, h, 0, E};
Point(5) = {w, 80, 0, e};
Point(6) = {w, 0, 0, E};

Point(7) = {xc, yc, 0, E};
Point(8) = {xc - r, yc, 0, e};
Point(9) = {xc, yc + r, 0, E};
Point(10) = {xc + r, yc, 0, e};
Point(11) = {xc, yc - r, 0, E};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 1};

Circle(7) = {8, 7, 9};
Circle(8) = {9, 7, 10};
Circle(9) = {10, 7, 11};
Circle(10) = {11, 7, 8};

Line(11) = {2, 8};
Line(12) = {5, 10};

Line Loop(1) = {1, 2, 3, 4, 5, 6};
Line Loop(2) = {7, 8, 9, 10};

Plane Surface(1) = {1, 2};
Line{11, 12} In Surface{1};

Physical Surface("all") = {1};
Physical Line("top") = {3};
Physical Line("bottom") = {6};
Physical Line("right") = {4, 5};
Physical Line("left") = {1, 2};
