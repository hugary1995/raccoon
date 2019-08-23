E = 0.05;
e = 0.005;

Point(1) = {-0.5, -0.5, 0, E};
Point(2) = {-0.5, -e/3, 0, e};
Point(3) = {-0.5, e/3, 0, e};
Point(4) = {-0.5, 0.5, 0, E};
Point(5) = {0.5, 0.5, 0, E};
Point(6) = {0.5, e/3, 0, e};
Point(7) = {0.5, -e/3, 0, e};
Point(8) = {0.5, -0.5, 0, E};

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
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};

Plane Surface(1) = {1};
Line{9, 10} In Surface {1};

Physical Surface("all") = {1};
Physical Line("top") = {4};
Physical Line("bottom") = {8};
Physical Line("left") = {1, 2, 3};
Physical Line("right") = {5, 6, 7};
