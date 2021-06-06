// inner radius
Ri = 14;

// outer radius
Ro = 25;

// height
H = 20;

// element sizes
e = 3;
ee = 0.5;

Point(1) = {0, 0, 0, e};

Point(2) = {Ri, 0, 0, ee};
Point(3) = {0, Ri, 0, ee};

Point(4) = {Ro, 0, 0, e};
Point(5) = {0, Ro, 0, e};

Circle(1) = {2, 1, 3};
Line(2) = {3, 5};
Circle(3) = {5, 1, 4};
Line(4) = {4, 2};

Line Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};

Extrude {0, 0, H} {
  Surface {1};
}

Physical Volume("metal") = {1};
Physical Surface("oxide_lower_dimensional_block") = {13};
Physical Surface("top") = {26};
Physical Surface("bottom") = {1};
Physical Surface("left") = {17};
Physical Surface("right") = {25};
Physical Surface("inner") = {13};
Physical Surface("outer") = {21};
