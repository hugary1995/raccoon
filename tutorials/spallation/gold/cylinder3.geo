// thickness of oxide
t = 0.2;

// inner radius
Ri = 14;

// outer radius
Ro = 25;

// height
H = 30;

// Oxide in-plane mesh size
ev = 0.25;
Nv = Ceil(H/ev)+1;
eh = 0.15;
Nh = Ceil((Ro-Ri)/eh)+1;

Point(0) = {0, 0, 0, 1};

Point(1) = {Ri-t, 0, 0, 1};
Point(2) = {0, Ri-t, 0, 1};

Point(3) = {Ri, 0, 0, 1};
Point(4) = {0, Ri, 0, 1};

Point(5) = {Ro, 0, 0, 1};
Point(6) = {0, Ro, 0, 1};

Circle(1) = {1, 0, 2};
Circle(2) = {3, 0, 4};
Circle(3) = {5, 0, 6};
Line(4) = {1, 3};
Line(5) = {3, 5};
Line(6) = {2, 4};
Line(7) = {4, 6};

Line Loop(1) = {4, 2, -6, -1};
Line Loop(2) = {5, 3, -7, -2};
Plane Surface(1) = {1};
Plane Surface(2) = {2};

Extrude {0, 0, H} {
  Surface {1, 2};
}

Transfinite Line {1, 2, 10, 12} = Nh;
Transfinite Line {14, 15, 19, 23} = Nv;
Transfinite Line {4, 6, 9, 11} = 2;
Transfinite Surface {1, 16, 20, 24, 28, 29};
Recombine Surface {1, 16, 20, 24, 28, 29};

Transfinite Line {3, 32} = Nh;
Transfinite Line {41, 37} = Nv;
Transfinite Line {5, 7, 31, -33} = 4 Using Progression 1.5;
Transfinite Surface {2, 51, 46, 42, 38};
Recombine Surface {2, 51, 46, 42, 38};

Transfinite Volume {1, 2};

Physical Volume("oxide") = {1};
Physical Volume("metal") = {2};
Physical Surface("top") = {51, 29};
Physical Surface("bottom") = {1, 2};
Physical Surface("left") = {24, 46};
Physical Surface("right") = {16, 38};
Physical Surface("inner") = {28};
Physical Surface("outer") = {42};
