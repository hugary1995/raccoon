L = 100;
H = 100;
D = 50; // crack depth
I = 25; // crack position (distance from bottom)
A = 62; // crack projectile angle
C = 75*Cos(A*Pi/180)+D;
t = 0.1;

E = 1.6; // coarse mesh size
e = 0.2; // refined mesh size

Point(1) = {0,0,0,E};
Point(2) = {0,I-t/2,0,E};
Point(3) = {D,I,0,e};
Point(4) = {0,I+t/2,0,E};
Point(5) = {0,H,0,E};
Point(6) = {C,H,0,e};
Point(7) = {L,H,0,E};
Point(8) = {L,0,0,E};

Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,5};
Line(5) = {5,6};
Line(6) = {6,7};
Line(7) = {7,8};
Line(8) = {8,1};

Line(9) = {3,6};

Line Loop(1) = {1,2,3,4,5,6,7,8};

Plane Surface(1) = {1};
Line{9} In Surface{1};

Physical Surface("all") = {1};
Physical Line("impact") = {1};
