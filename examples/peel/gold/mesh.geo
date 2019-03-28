L = 1;
H = 1;
D = 0.2;
t = 0.005;

E = 0.1;
e = 0.002;

Point(1) = {0,0,0,E};
Point(2) = {0,H/2-t/2,0,e};
Point(3) = {D,H/2,0,e};
Point(4) = {D,H/2-t/2,0,e};
Point(5) = {D+t/2,H/2,0,e};
Point(6) = {D,H/2+t/2,0,e};
Point(7) = {0,H/2+t/2,0,e};
Point(8) = {0,H,0,E};
Point(9) = {L,H,0,E};
Point(10) = {L,H/2,0,e};
Point(11) = {L,0,0,E};

Line(1) = {1,2};
Line(2) = {2,4};
Circle(3) = {4,3,5};
Circle(4) = {5,3,6};
Line(5) = {6,7};
Line(6) = {7,8};
Line(7) = {8,9};
Line(8) = {9,10};
Line(9) = {10,11};
Line(10) = {11,1};

Line(11) = {5,10};

Line Loop(1) = {1,2,3,4,5,6,7,8,9,10};

Plane Surface(1) = {1};
Line{11} In Surface{1};

Physical Surface("all") = {1};
Physical Line("bottom") = {10};
Physical Line("top") = {7};
Physical Line("right_upper") = {8};
Physical Line("right_lower") = {9};
