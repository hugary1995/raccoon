Mesh.MshFileVersion = 2.2;
Mesh.Smoothing = 100;
//Mesh.Algorithm = 8; // Delaunay for quads
e = 120;
ee = 120;

Point(1) = {-8, -8, 0};
Point(2) = {-8, 8, 0};
Point(3) = {8, 8, 0};
Point(4) = {8, -8, 0};
Point(5) = {0,0,0};

Point(6) = {-2.26,-2.26,0};
Point(7) = {-2.26,2.26,0};
Point(8) = {2.26, 2.26,0};
Point(9) = {2.26, -2.26,0};

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


//Transfinite Curve{5, 4, -6, -10} = e Using Progression 1;
//Transfinite Curve{6, 1, 7, -11} = e Using Progression 1;
//Transfinite Curve{2, 8, -12, -7} = e Using Progression 1;
//Transfinite Curve{8, 13, 5, -3} = e Using Progression 1;

//Transfinite Surface{1};
//Transfinite Surface{2};
//Transfinite Surface{3};
//Transfinite Surface{4};

//Recombine Surface{1,2,3,4};


//+
//+
//+
Extrude {0, 0, .1} {
  Curve{12}; Curve{13}; Curve{10}; Curve{11}; Curve{6}; Curve{4}; Curve{5}; Curve{3}; Curve{2}; Curve{1}; Curve{7}; Curve{8};
}


//Transfinite Curve{5, 4, -6, -10} = e Using Progression 1;
//Transfinite Curve{6, 1, 7, -11} = e Using Progression 1;
//Transfinite Curve{2, 8, -12, -7} = e Using Progression 1;
//Transfinite Curve{8, 13, 5, -3} = e Using Progression 1;



//Transfinite Surface{1};
//Transfinite Surface{2};
//Transfinite Surface{3};
//Transfinite Surface{4};

//+
Curve Loop(5) = {12, 16, -14, -15};
Curve Loop(6) = {11, 15, -26, -24};
Curve Loop(7) = {10, 24, -22, -20};
Curve Loop(8) = {13, 20, -18, -16};
Curve Loop(9) = {22, 30, -34, -38};
Curve Loop(10) = {30, 50, 54, -26};
Curve Loop(11) = {46, 58, -14, -54};
Curve Loop(12) = {38, -42, 58, 18};



Curve Loop(1) = {5, 4, -6, -10};
Curve Loop(2) = {6, 1, 7, -11};
Curve Loop(3) = {2, 8, -12, -7};
Curve Loop(4) = {8, 13, 5, -3};


Plane Surface(70) = {1};
Plane Surface(71) = {2};
Plane Surface(72) = {3};
Plane Surface(73) = {4};

Plane Surface(62) = {5};
Plane Surface(69) = {11};
Plane Surface(68) = {12};
Plane Surface(67) = {10};
Plane Surface(65) = {8};
Plane Surface(64) = {7};
Plane Surface(63) = {6};
Plane Surface(66) = {9};

//Transfinite Curve{12, 16, -14, -15} = ee Using Progression 1;
//Transfinite Curve{11, 15, -26, -24} = ee Using Progression 1;
//Transfinite Curve{10, 24, -22, -20} = ee Using Progression 1;
//Transfinite Curve{13, 20, -18, -16} = ee Using Progression 1;
//Transfinite Curve{22, 30, -34, -38} = e Using Progression 1;
//Transfinite Curve{30, 50, 54, -26} = e Using Progression 1;
//Transfinite Curve{46, 58, -14, -54} = e Using Progression 1;
//Transfinite Curve{38, -42, 58, 18} = e Using Progression 1;

//Transfinite Surface{62};
//Transfinite Surface{69};
//Transfinite Surface{68};
//Transfinite Surface{67};
//Transfinite Surface{65};
//Transfinite Surface{64};
//Transfinite Surface{63};
//Transfinite Surface{66};



//+
//+
Surface Loop(1) = {67, 53, 71, 57, 33, 29};
//+
Volume(1) = {1};
//+
Surface Loop(2) = {33, 37, 70, 66, 41, 25};
//+
Volume(2) = {2};
//+
Surface Loop(3) = {41, 21, 61, 45, 73, 68};
//+
Volume(3) = {3};
//+
Surface Loop(4) = {17, 57, 49, 72, 69, 61};
Volume(4) = {4};


Transfinite Volume{1};// = e Using Progression 1;
Transfinite Volume{2};// = e Using Progression 1;
Transfinite Volume{3};// = e Using Progression 1;
Transfinite Volume{4};// = e Using Progression 1;

Recombine Volume{1,2,3,4};
