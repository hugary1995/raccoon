a = 20;
h = 1;
H = 1;
e = 0.1;
E = 0.1;

Point(1) = {0,0,0,e};
Point(2) = {0,h,0,e};
Point(3) = {a,h,0,e};
Point(4) = {a,0,0,e};
Point(5) = {0,-H,0,E};
Point(6) = {a,-H,0,E};

Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};
Line(5) = {1,5};
Line(6) = {5,6};
Line(7) = {6,4};

Line Loop(1) = {1,2,3,4};
Line Loop(2) = {5,6,7,4};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Transfinite Line {1,5} = 11 Using Progression 1;
Transfinite Line {2,6} = 201 Using Progression 1;
Transfinite Surface {1,2};
Recombine Surface {1,2};

Physical Surface("film") = {1};
Physical Surface("substrate") = {2};
Physical Line("bottom") = {6};
Physical Line("left") = {1,5};
Physical Line("right") = {3,7};
Physical Line("top") = {2};
