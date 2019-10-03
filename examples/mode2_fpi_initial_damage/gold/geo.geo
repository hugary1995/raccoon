E = 0.05;
e = 0.005;

Point(1) = {-0.5, -0.5, 0, E};
Point(2) = {-0.5, 0, 0, e};
Point(3) = {-0.5, 0.5, 0, E};
Point(4) = {0.5, 0.5, 0, E};
Point(5) = {0.5, 0, 0, E};
Point(6) = {0.5, -0.5+E, 0, e};
Point(7) = {0.5, -0.5, 0, e};
Point(8) = {0, 0, 0, e};

Point(9) = {0, -0.0025, 0, e};
Point(10) = {0.4, -0.5 , 0, e};
Point(11) = {0.1, -0.28, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 10};
Line(8) = {10, 1};
Line(9) = {2, 8};
BSpline(10) = {9, 11, 10};
Line(11) = {5, 8};
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};

Plane Surface(1) = {1};
Line{9, 10, 11} In Surface {1};
// Transfinite Line {6} = 21 Using Progression 1;

Physical Surface("all") = {1};
Physical Line("top") = {3};
Physical Line("bottom") = {7, 8};
Physical Line("left") = {1, 2};
Physical Line("right") = {4, 5, 6};
