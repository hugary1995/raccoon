E = 0.05;
e = 0.005;

Point(1) = {-0.5, -0.5, 0, E};
Point(2) = {-0.5, -0.0025, 0, E};
Point(3) = {-0.5, 0.0025, 0, E};
Point(4) = {-0.5, 0.5, 0, E};
Point(5) = {0.5, 0.5, 0, E};
Point(6) = {0.5, -0.5, 0, E};

Point(7) = {0, -0.0025, 0, e};
Point(8) = {0.4, -0.5 , 0, e};
Point(9) = {0.1, -0.28, 0, e};

Point(10) = {-0.0025, -0.0025, 0, e};
Point(11) = {-0.0025, 0.0025, 0, E};
Point(12) = {-0.0025, -0, 0, E};

Line(1) = {1, 8};
Line(2) = {8, 6};
Line(3) = {6, 5};
Line(4) = {5, 4};
Line(5) = {4, 3};
Line(6) = {3, 11};
Line(7) = {10, 2};
Line(8) = {2, 1};
Circle(9) = {10, 12, 11};
Line Loop(9) = {5, 6, -9, 7, 8, 1, 2, 3, 4};

Plane Surface(10) = {9};
BSpline(11) = {7, 9, 8};
Line{11} In Surface {10};
Transfinite Line {2} = 21 Using Progression 1;

Physical Surface("all") = {10};
Physical Line("top") = {4};
Physical Line("bottom") = {1,2};
Physical Line("left") = {5,8};
Physical Line("right") = {3};
