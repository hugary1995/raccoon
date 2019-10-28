a = 0.5;
r = 0.0025;
d0 = -0.4;

E = 0.05;
e = 0.005;

Point(1) = {-a, -a, 0, E};
Point(2) = {-a, -r, 0, E};
Point(3) = {d0-r, -r, 0, e};
Point(4) = {d0-r, 0, 0, e};
Point(5) = {d0-r, r, 0, e};
Point(6) = {-a, r, 0, E};
Point(7) = {-a, 0.5, 0, E};
Point(8) = {a, a, 0, E};
Point(9) = {a, 5*r, 0, e};
Point(10) = {a, -5*r, 0, e};
Point(11) = {a, -a, 0, E};

Point(12) = {d0-r, 5*r, 0, e};
Point(13) = {d0-r, -5*r, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Circle(3) = {3, 4, 5};
Line(4) = {5, 6};
Line(5) = {6, 7};
Line(6) = {7, 8};
Line(7) = {8, 9};
Line(8) = {9, 10};
Line(9) = {10, 11};
Line(10) = {11, 1};
Line(11) = {12, 9};
Line(12) = {13, 10};
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

Plane Surface(1) = {1};
Line{11, 12} In Surface {1};

Physical Surface("all") = {1};
Physical Line("top") = {6};
Physical Line("bottom") = {10};
Physical Line("left") = {1, 5};
Physical Line("right") = {7, 8, 9};
