H = 0.3;
h = 0.008;
l = 0.04;
L = 4;
c = 0.3;

Point(1) = {-L/2, -L/2, 0, H};
Point(2) = {-c, -l, 0, h};
Point(3) = {-c, l, 0, h};
Point(4) = {-L/2, L/2, 0, H};
Point(5) = {L/2, L/2, 0, H};
Point(6) = {c, l, 0, h};
Point(7) = {c, -l, 0, h};
Point(8) = {L/2, -L/2, 0, H};

Line(1) = {1, 4};
Line(2) = {4, 5};
Line(3) = {5, 8};
Line(4) = {8, 1};
Line(5) = {2, 3};
Line(6) = {3, 6};
Line(7) = {6, 7};
Line(8) = {7, 2};

Line Loop(1) = {1, 2, 3, 4};
//Line Loop(2) = {5, 6, 7, 8} 


Plane Surface(1) = {1};
Line{5, 6, 7, 8} In Surface {1};

Physical Surface("all") = {1};
Physical Line("top") = {2};
Physical Line("bottom") = {4};
Physical Line("left") = {1};
Physical Line("right") = {3};
