e = 0.2;

W = 1;
H = 1;

Point(1) = {0, 0, 0, e};
Point(2) = {0, H, 0, e};
Point(3) = {W, H, 0, e};
Point(4) = {W, 0, 0, e};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

Line Loop(1) = {1, 2, 3, 4};

Plane Surface(1) = {1};

Extrude {0, 0, 1} {
  Surface{1};
}

Physical Volume("all") = {1};
Physical Surface("top") = {17};
Physical Surface("bottom") = {25};
