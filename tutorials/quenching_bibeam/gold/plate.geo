// Thickness of 304 stainless steel
t1 = 3.17;

// Thickness of borosilicate glass
t2 = 31.7;

// Length of the plate
L = 300;

// X-offset of the crack
X = 30;

// Length of the crack
Y = 10;

// Element sizes
e1 = 4;
e2 = 2;

eps = 1e-2;

Point(1) = {0, 0, 0, e2};
Point(2) = {0, t2, 0, e2};
Point(3) = {L, t2, 0, e2};
Point(4) = {L, 0, 0, e2};
Point(5) = {X+eps, 0, 0, e2};
Point(6) = {X, Y, 0, e2};
Point(7) = {X-eps, 0, 0, e2};
Point(8) = {0, t1+t2, 0, e1};
Point(9) = {L, t1+t2, 0, e1};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 1};

Line(8) = {2, 8};
Line(9) = {8, 9};
Line(10) = {9, 3};

Line Loop(1) = {1, 2, 3, 4, 5, 6, 7};
Line Loop(2) = {8, 9, 10, -2};

Plane Surface(1) = {1};
Plane Surface(2) = {2};

Transfinite Line {2} = Ceil(L/e2)+1;
Transfinite Line {9} = Ceil(L/e1)+1;
Transfinite Line {8, 10} = Ceil(t1/e1)+1;

Physical Surface("glass") = {1};
Physical Surface("steel") = {2};
Physical Point("left") = {8};
Physical Point("right") = {9};
