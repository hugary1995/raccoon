Mesh.MshFileVersion = 2.2;
Mesh.Smoothing = 100;

Point(2) = {.15, 0, 0, 1};

Point(3) = {0, .15, 0, 1};

Point(4) = {0.08, 0, 0, 1};

//Point(5) = {.05713, .056, 0, 1};

//Point(6) = {0.056, .05713, 0, 1};


Point(5) = {0.056569, 0.056569, 0, 1};
Point(6) = {0.056569, 0.056569, 0, 1};


Point(7) = {.081317, .081317, 0, 1};
Point(8) = {0,0.08,0,1};
Point(9) = {0.10606,0.10606,0,1};
Point(10) = {0.115,0,0,1};
Point(11) = {0,0.115,0,1};
//+
Point(12) = {0, 0, 0, 1};
//+
Circle(1) = {8, 12, 6};
//+

//+
Circle(2) = {5, 12, 4};
//+
Circle(3) = {2, 12, 9};
//+
Circle(4) = {9, 12, 3};
//+

//+
Line(6) = {4, 10};
//+
Line(7) = {10, 2};
//+
Line(8) = {3, 11};
//+
Line(9) = {11, 8};
//+
Line(10) = {6, 7};
//+
Line(11) = {5, 7};
//+
Circle(12) = {11, 12, 7};
//+
Circle(13) = {10, 12, 7};
//+
Line(14) = {7, 9};
//+
Curve Loop(1) = {4, 8, 12, 14};
//+
Surface(1) = {1};
//+
Curve Loop(2) = {12, -10, -1, -9};
//+
Surface(2) = {2};
//+
Curve Loop(3) = {13, 14, -3, -7};
//+
Surface(3) = {3};
//+
Curve Loop(4) = {2, 6, 13, -11};
//+


Surface(4) = {4};




Transfinite Curve{4, 8, 12, 14} = 150 Using Progression 1;
Transfinite Curve{12, -10, -1, -9} = 150 Using Progression 1;
Transfinite Curve{13, 14, -3, -7} = 150 Using Progression 1;
Transfinite Curve{2, 6, 13, -11} = 150 Using Progression 1;

Transfinite Surface{1};
Transfinite Surface{2};
Transfinite Surface{3};
Transfinite Surface{4};

Recombine Surface{1,2,3,4};

Physical Line("Vertical") = {8,9};
Physical Line("Horizontal") = {6,7};
Physical Surface("Face") = {1,2,3,4};
Physical Line("Inner") = {1,2};



