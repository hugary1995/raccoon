E = 0.05;
e = 0.005;

Point(1) = {-0.5, -0.5, 0, E};
Point(2) = {-0.5, 0, 0, e};
Point(3) = {-0.5, 0.5, 0, E};
Point(4) = {0.5, 0.5, 0, E};
Point(5) = {0.5, 0, 0, E};
Point(6) = {0.5, -0.5, 0, E};
Point(7) = {0, 0, 0, e};

Point(8) = {0, -0.0025, 0, e};
Point(9) = {0.4, -0.5 , 0, e};
Point(10) = {0.1, -0.28, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 9};
Line(7) = {9, 1};
Line(8) = {2, 7};
BSpline(9) = {8, 10, 9};
Line(10) = {5,7};
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7};

Plane Surface(1) = {1};
Line{8, 9, 10} In Surface {1};
Transfinite Line {6} = 21 Using Progression 1;

Physical Surface("all") = {1};
Physical Line("top") = {3};
Physical Line("bottom") = {6, 7};
Physical Line("left") = {1, 2};
Physical Line("right") = {4, 5};
