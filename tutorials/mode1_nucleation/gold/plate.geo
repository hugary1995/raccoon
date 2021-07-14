// Width
W = 5;

// Length
L = 7.5;

// Element sizes
e = 1;
ee = 0.0001;

Point(1) = {0, 0, 0, ee};
Point(2) = {0, L, 0, e};
Point(3) = {W, L, 0, e};
Point(4) = {W, 0, 0, e};
Point(5) = {a+10*ee, 0, 0, ee};
Point(6) = {a, 0, 0, ee};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 1};

Line Loop(1) = {1, 2, 3, 4, 5, 6};

Plane Surface(1) = {1};

n = Round(a/ee);
Transfinite Line {6} = n+1 Using Progression 1;
Transfinite Line {5} = 11+Modulo(n,2);

Physical Surface("all") = {1};
Physical Line("left") = {1};
Physical Line("right") = {3};
Physical Line("top") = {2};
Physical Line("noncrack") = {4, 5};
