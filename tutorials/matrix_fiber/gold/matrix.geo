// Cross-section side length of the matrix
L = 3;

// Diameter of the cynlinder
R = 0.5;

// Refinement line parameters
angle1 = -20*Pi/180;
angle2 = 0*Pi/180;
eps = 0.2;

// Element sizes
e = 0.4;
ee = 0.02;

Point(1) = {0, 0, 0, e};
Point(2) = {0, L/2+R*Sin(angle1), 0, ee};
Point(3) = {0, L/2+(R+eps)*Sin(angle2), 0, ee};
Point(4) = {0, L, 0, e};

Point(5) = {L, L, 0, e};
Point(6) = {L, L/2+(R+eps)*Sin(angle2), 0, ee};
Point(7) = {L, L/2+R*Sin(angle1), 0, ee};
Point(8) = {L, 0, 0, e};

Point(9) = {L/2, L/2, 0, e};
Point(10) = {L/2-R*Cos(angle1), L/2+R*Sin(angle1), 0, ee};
Point(11) = {L/2, L/2+R, 0, ee};
Point(12) = {L/2+R*Cos(angle1), L/2+R*Sin(angle1), 0, ee};
Point(13) = {L/2, L/2-R, 0, e};

Point(14) = {L/2-(R+eps)*Cos(angle2), L/2+(R+eps)*Sin(angle2), 0, ee};
Point(15) = {L/2, L/2+R+eps, 0, ee};
Point(16) = {L/2+(R+eps)*Cos(angle2), L/2+(R+eps)*Sin(angle2), 0, ee};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 1};

Circle(9) = {10, 9, 11};
Circle(10) = {11, 9, 12};
Circle(11) = {12, 9, 13};
Circle(12) = {13, 9, 10};

Circle(13) = {14, 9, 15};
Circle(14) = {15, 9, 16};

Line(15) = {2, 10};
Line(16) = {3, 14};
Line(17) = {6, 16};
Line(18) = {7, 12};

Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};
Line Loop(2) = {9, 10, 11, 12};

Plane Surface(1) = {1, 2};
Line {13, 14, 15, 16, 17, 18} In Surface {1};

Physical Surface("all") = {1};
Physical Line("left") = {1, 2, 3};
Physical Line("right") = {5, 6, 7};
Physical Line("top") = {4};
Physical Line("hole") = {9, 10, 11, 12};
