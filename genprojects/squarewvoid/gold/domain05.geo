Mesh.MshFileVersion = 2.2;
Mesh.Smoothing = 100;
//Mesh.Algorithm = 8; // Delaunay for quads
e = 250;

Point(1) = {-1.5, -1.5, 0};
Point(2) = {-1.5, 1.5, 0};
Point(3) = {1.5, 1.5, 0};
Point(4) = {1.5, -1.5, 0};
Point(5) = {0,0,0};

Point(6) = {-0.3535,-0.3535,0};
Point(7) = {-0.3535,0.3535,0};
Point(8) = {0.3535, 0.3535,0};
Point(9) = {0.3535, -0.3535,0};

Circle(1) = {9,5,8}; //Sectioned inner cirlce
Circle(2) = {8,5,7};
Circle(3) = {7,5,6};
Circle(4) = {6,5,9};

Line(5) = {1,6}; //1 //Inner Lines
Line(6) = {4,9}; //2
Line(7) = {8,3}; //3
Line(8) = {7,2}; //4

Line(10) = {1,4}; //on surface 1  //Outer Lines
Line(11) = {4,3}; //''2
Line(12) = {3,2}; //''3
Line(13) = {2,1}; //''4

Curve Loop(1) = {5, 4, -6, -10};
Curve Loop(2) = {6, 1, 7, -11};
Curve Loop(3) = {2, 8, -12, -7};
Curve Loop(4) = {8, 13, 5, -3};


Plane Surface(1) = {1};
Plane Surface(2) = {2};
Plane Surface(3) = {3};
Plane Surface(4) = {4};


Transfinite Curve{5, 4, -6, -10} = e Using Progression 1;
Transfinite Curve{6, 1, 7, -11} = e Using Progression 1;
Transfinite Curve{2, 8, -12, -7} = e Using Progression 1;
Transfinite Curve{8, 13, 5, -3} = e Using Progression 1;

Transfinite Surface{1};
Transfinite Surface{2};
Transfinite Surface{3};
Transfinite Surface{4};

Recombine Surface{1,2,3,4};

Physical Point("BottomLeft") = {1};
Physical Line("Top") = {12};
Physical Line("Bottom") = {10};
Physical Line("Right") = {11};
Physical Line("Left") = {13};
Physical Line("Hole") = {1,2,3,4};
Physical Surface("Domain") = {1,2,3,4};
