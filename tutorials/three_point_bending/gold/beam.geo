// center of the smaller hole
O1x = 92;
O1y = 7;
R1 = 2;

// center of the larger hole
O2x = 100;
O2y = 9;
R2 = 3;

// element sizes
e = 10;
ee = 2;
eee = 0.4;
eeee = 0.3;
eeeee = 0.04;

Point(1) = {0, 0, 0, e};
Point(2) = {20, 0, 0, e};

Point(3) = {96, 0, 0, ee};
Point(4) = {93, 0, 0, ee};
Point(23) = {96+3*Cos(115*Pi/180), 0+3*Sin(115*Pi/180), 0, eeeee};
Point(5) = {95.865, 2.99874, 0, eeeee};
Point(24) = {96+3*Cos(75*Pi/180), 0+3*Sin(75*Pi/180), 0, eeee};
Point(6) = {99, 0, 0, ee};

Point(7) = {180, 0, 0, e};
Point(8) = {200, 0, 0, e};
Point(9) = {200, 16, 0, e};
Point(10) = {100, 16, 0, e};
Point(11) = {92, 16, 0, e};
Point(12) = {0, 16, 0, e};

Point(13) = {O1x, O1y, 0, ee};
Point(14) = {O1x-R1, O1y, 0, ee};
Point(15) = {O1x+R1*Cos(-92*Pi/180), O1y+R1*Sin(-92*Pi/180), 0, eeeee};
//Point(16) = {O1x+R1, O1y, 0, ee};
Point(16) = {O1x+R1*Cos(-65*Pi/180), O1y+R1*Sin(-65*Pi/180), 0, eeeee};
Point(17) = {O1x, O1y+R1, 0, ee};

Point(18) = {O2x, O2y, 0, ee};
Point(19) = {O2x+R2*Cos(-95*Pi/180), O2y+R2*Sin(-95*Pi/180), 0, eeee};
Point(20) = {O2x+R2*Cos(-86*Pi/180), O2y+R2*Sin(-86*Pi/180), 0, eeee};
Point(21) = {O2x+R2, O2y, 0, ee};
Point(22) = {O2x, O2y+R2, 0, ee};

Line(1) = {1, 2};
Line(2) = {2, 4};
Circle(3) = {4, 3, 23};
Circle(25) = {23, 3, 5};
Circle(26) = {5, 3, 24};
Circle(4) = {24, 3, 6};
Line(5) = {6, 7};
Line(6) = {7, 8};
Line(7) = {8, 9};
Line(8) = {9, 10};
Line(9) = {10, 11};
Line(10) = {11, 12};
Line(11) = {12, 1};

Circle(12) = {14, 13, 15};
Circle(13) = {15, 13, 16};
Circle(14) = {16, 13, 17};
Circle(15) = {17, 13, 14};

Circle(16) = {19, 18, 20};
Circle(17) = {20, 18, 21};
Circle(18) = {21, 18, 22};
Circle(19) = {22, 18, 19};

Line(20) = {23, 15};
Line(21) = {5, 16};

Line Loop(1) = {1, 2, 3, 25, 26, 4, 5, 6, 7, 8, 9,10, 11};
Line Loop(2) = {12, 13, 14, 15};
Line Loop(3) = {16, 17, 18, 19};

Plane Surface(1) = {1, 2, 3};
Line {20, 21} In Surface {1};

Physical Surface("all") = {1};
Physical Point("left") = {2};
Physical Point("right") = {7};
Physical Point("top") = {10};
Physical Point("sensor") = {6};
