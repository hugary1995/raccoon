e = 0.5;

r = 4.5;
R = 5;
H = 12;

Point(0) = {0, 0, 0, e};
Point(1) = {0, r, 0, e};
Point(2) = {r, 0, 0, e};
Point(3) = {0, -r, 0, e};
Point(4) = {-r, 0, 0, e};
Point(5) = {0, R, 0, e};
Point(6) = {R, 0, 0, e};
Point(7) = {0, -R, 0, e};
Point(8) = {-R, 0, 0, e};

Circle(1) = {1, 0, 2};
Circle(2) = {2, 0, 3};
Circle(3) = {3, 0, 4};
Circle(4) = {4, 0, 1};
Circle(5) = {5, 0, 6};
Circle(6) = {6, 0, 7};
Circle(7) = {7, 0, 8};
Circle(8) = {8, 0, 5};

Line Loop(1) = {1, 2, 3, 4};
Line Loop(2) = {5, 6, 7, 8};

Plane Surface(1) = {1, 2};

Extrude {0, 0, H} {
  Surface{1};
}

Physical Volume("all") = {1};
Physical Surface("bottom") = {1};
Physical Surface("top") = {50};
Physical Surface("inner") = {21, 25, 29, 33};
Physical Point("pin") = {5};
