// center of the smaller hole
O1x = 92;
O1y = 7;
R1 = 2;

// center of the larger hole
O2x = 100;
O2y = 9;
R2 = 3;

// element sizes
e = 5;
ee = 1;
eee = 0.12;

Point(1) = {0, 0, 0, e};
Point(2) = {20, 0, 0, e};

Point(3) = {96, 0, 0, ee};
Point(4) = {93, 0, 0, eee};
Point(5) = {96, 3, 0, eee};
Point(6) = {99, 0, 0, eee};

Point(7) = {180, 0, 0, e};
Point(8) = {200, 0, 0, e};
Point(9) = {200, 16, 0, e};
Point(10) = {100, 16, 0, e};
Point(11) = {92, 16, 0, ee};
Point(12) = {0, 16, 0, e};

Point(13) = {O1x, O1y, 0, ee};
Point(14) = {O1x-R1, O1y, 0, ee};
Point(15) = {O1x, O1y-R1, 0, eee};
Point(16) = {O1x+R1, O1y, 0, eee};
Point(17) = {O1x, O1y+R1, 0, eee};

Point(18) = {O2x, O2y, 0, ee};
Point(19) = {O2x-R2, O2y, 0, eee};
Point(20) = {O2x, O2y-R2, 0, eee};
Point(21) = {O2x+R2, O2y, 0, ee};
Point(22) = {O2x, O2y+R2, 0, ee};

Line(1) = {1, 2};
Line(2) = {2, 4};
Circle(3) = {4, 3, 5};
Circle(4) = {5, 3, 6};
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

Line(20) = {4, 15};
Line(21) = {5, 16};
Line(22) = {5, 19};
Line(23) = {6, 20};
Line(24) = {17, 11};
Line(25) = {16, 19};

Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11};
Line Loop(2) = {12, 13, 14, 15};
Line Loop(3) = {16, 17, 18, 19};

Plane Surface(1) = {1, 2, 3};
Line {20, 21, 22, 23, 24, 25} In Surface {1};

Physical Surface("all") = {1};
Physical Point("left") = {2};
Physical Point("right") = {7};
Physical Point("top") = {10};
