Mesh.MshFileVersion = 2.2;
E = .01;
e = .005;
Point(1) = {0, 0, 0, E};
Point(2) = {0, 0.08, 0, e};
Point(3) = {0, -0.08, 0, e};
Point(4) = {0, 0.15, 0, E};
Point(5) = {0, -0.15, 0, E};

Circle(1) = {2, 1, 3};
Circle(2) = {3, 1, 2};
Circle(3) = {4, 1, 5};
Circle(4) = {5, 1, 4};

Line Loop(1) = {1, 2};
Line Loop(2) = {3, 4};

Plane Surface(1) = {1, 2};

Physical Line("Outer") = {3,4};
Physical Line("Inner") = {1,2};
Physical Surface("Vessel") = {1};
