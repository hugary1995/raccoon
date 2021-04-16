Mesh.MshFileVersion = 2.2;
Mesh.Smoothing = 100;
//Mesh.Algorithm = 8; // Delaunay for quads
e =.02;
E = .02;

Point(1) = {-1.5, -1.5, 0,E};
Point(2) = {-1.5, 1.5, 0,E};
Point(3) = {1.5, 1.5, 0,E};
Point(4) = {1.5, -1.5, 0,E};
Point(5) = {0,0,0,E};

Point(6) = {-0.353553,-0.353553,0,e};
Point(7) = {-0.353553,0.353553,0,e};
Point(8) = {0.353553, 0.353553,0,e};
Point(9) = {0.353553, -0.353553,0,e};
Point(10) = {-1.49, 0 ,0 , e};
Point(11) = {1.49, 0 , 0 , e};


//Point(16) = {0,1,0,e};
//Point(17) = {0,1,0,e};


//Point(22) = {-1,0.1294,0,e};
//Point(23) = {1,0.1294,0,e};

Circle(1) = {9,5,8}; //Sectioned inner cirlce
Circle(2) = {8,5,7};
Circle(3) = {7,5,6};
Circle(4) = {6,5,9};

//Circle(5) = {17,5,12};
//Circle(6) = {16,5,13};

Line(10) = {1,4}; //on surface 1  //Outer Lines
Line(11) = {4,3}; //''2
Line(12) = {3,2}; //''3
Line(13) = {2,1}; //''4


//Line(16) = {16,19};

//Line(18) = {20,22};
//Line(19) = {21,23};
Curve Loop(1) = {12, 13, 10, 11};
//+
Curve Loop(2) = {3, 4, 1, 2};
//+
Plane Surface(1) = {1, 2};


//Circle{5} In Surface{1};
//Circle{6} In Surface{1};

Physical Point("BottomLeft") = {1};
Physical Line("Top") = {12};
Physical Line("Bottom") = {10};
Physical Line("Right") = {11};
Physical Line("Left") = {13};
Physical Line("Hole") = {1,2,3,4};
Physical Surface("Domain") = {1,2,3,4};
//+
