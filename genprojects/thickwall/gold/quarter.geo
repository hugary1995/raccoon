Mesh.MshFileVersion = 2.2;
Mesh.Smoothing = 100;
//Mesh.Algorithm = 8; // Delaunay for quads

E = .01;
e = .005;
Point(1) = {0, 0, 0, e};
Point(2) = {0, 0.08, 0, e};
Point(3) = {0, 0.15, 0, e};
Point(4) = {0.15, 0, 0, e};
Point(5) = {0.08, 0, 0, e};

Circle(1) = {5, 1, 2};
Circle(2) = {3, 1, 4};
Line(3) = {2,3};
Line(4) = {4,5};

Curve Loop(1) = {1,3,2,4};


Plane Surface(1) = {1};
Physical Surface(1) = {1};

Transfinite Curve{1,3,2,4} = 300  Using Progression 1;
Transfinite Surface{1};

Recombine Surface{1};

Physical Line("Vertical") = {3};
Physical Line("Horizontal") = {4};
Physical Line("Outer") = {2};
Physical Line("Inner") = {1};
Physical Surface("Vessel") = {1};
