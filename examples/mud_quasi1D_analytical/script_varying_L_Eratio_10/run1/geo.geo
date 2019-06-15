h = 1.000;
L = 2.511886E+00;
H = h*5;
e = L/200;
E = e*10;
r = e;

Point(0) = {L/2,h,0,e};
Point(1) = {0,0,0,E};
Point(2) = {0,h,0,E};
Point(3) = {L/2-r,h,0,e};
Point(4) = {L/2,h-r,0,e};
Point(5) = {L/2+r,h,0,e};
Point(6) = {L,h,0,E};
Point(7) = {L,0,0,E};
Point(8) = {L/2,0,0,e};
Point(9) = {0,-H,0,E};
Point(10) = {L,-H,0,E};

Line(1) = {1,2};
Line(2) = {2,3};
Circle(3) = {3,0,4};
Circle(4) = {4,0,5};
Line(5) = {5,6};
Line(6) = {6,7};
Line(7) = {7,8};
Line(8) = {8,1};
Line(9) = {1,9};
Line(10) = {9,10};
Line(11) = {10,7};
Line(12) = {4,8};

Line Loop(1) = {1,2,3,4,5,6,7,8};
Line Loop(2) = {9,10,11,7,8};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Line{12} In Surface{1};

Physical Surface("film") = {1};
Physical Surface("substrate") = {2};
Physical Line("bottom") = {10};
Physical Line("left") = {1,9};
Physical Line("right") = {6,11};
