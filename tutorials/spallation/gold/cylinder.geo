// inner radius
Ri = 14;

// outer radius
Ro = 25;

// height
H = 20;

// element sizes
e = 4;
ee = 1.2;
eee = 0.2;

Point(1) = {0, 0, 0, e};

Point(2) = {Ri, 0, 0, e};
Point(3) = {0, Ri, 0, e};
Point(4) = {-Ri, 0, 0, e};
Point(5) = {0, -Ri, 0, e};

Point(6) = {Ro, 0, 0, e};
Point(7) = {0, Ro, 0, e};
Point(8) = {-Ro, 0, 0, e};
Point(9) = {0, -Ro, 0, e};

Circle(1) = {2, 1, 3};
Circle(2) = {3, 1, 4};
Circle(3) = {4, 1, 5};
Circle(4) = {5, 1, 2};

Circle(5) = {6, 1, 7};
Circle(6) = {7, 1, 8};
Circle(7) = {8, 1, 9};
Circle(8) = {9, 1, 6};

Line Loop(1) = {1, 2, 3, 4};
Line Loop(2) = {5, 6, 7, 8};
Plane Surface(1) = {1, 2};

Extrude {0, 0, H} {
  Surface {1};
}

Physical Volume("metal") = {1};
Physical Surface("bottom") = {1};
Physical Surface("top") = {50};
Physical Surface("inner") = {21, 25, 29, 33};
Physical Surface("outer") = {37, 41, 45, 49};
Physical Surface("oxide_lower_dimensional_block") = {21, 25, 29, 33};
