a = 0.5;
r = 0.0025;

E = 0.05;
e = 0.005;

Point(1) = {-a, -a, 0, E};
Point(2) = {-a, -r, 0, E};
Point(3) = {-r, -r, 0, e};
Point(4) = {-r, 0, 0, e};
Point(5) = {-r, r, 0, e};
Point(6) = {-a, r, 0, E};
Point(7) = {-a, a, 0, E};
Point(8) = {a, a, 0, E};
Point(9) = {a, -a+10*r, 0, e};
Point(10) = {a, -a, 0, e};
Point(11) = {0.4, -a, 0, e};
Point(12) = {0, -0.0025, 0, e};
Point(13) = {0.1, -0.28, 0, e};
Point(14) = {a, -0.05, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Circle(3) = {3, 4, 5};
Line(4) = {5, 6};
Line(5) = {6, 7};
Line(6) = {7, 8};
Line(7) = {8, 14};
Line(12) = {14, 9};
Line(8) = {9, 10};
Line(9) = {10, 11};
Line(10) = {11, 1};
BSpline(11) = {12, 13, 11};
Line(13) = {12, 14};
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 12, 8, 9, 10};

Plane Surface(1) = {1};
Line{11, 13} In Surface {1};

Physical Surface("all") = {1};
Physical Line("top") = {6};
Physical Line("bottom") = {9, 10};
Physical Line("left") = {1, 5};
Physical Line("right") = {7, 12, 8};
