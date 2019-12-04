w = 2;
h = 2;

E = 0.1;
L = 0.03;
c = 0.07;
e = L*L/c;

Point(1) = {0, 0, 0, E};
Point(2) = {0, h/2-2*L, 0, e};
Point(3) = {0, h/2, 0, e};
Point(4) = {0, h/2+2*L, 0, e};
Point(5) = {0, h, 0, E};
Point(6) = {w, h, 0, E};
Point(7) = {w, h/2+2*L, 0, e};
Point(8) = {w, h/2, 0, e};
Point(9) = {w, h/2-2*L, 0, e};
Point(10) = {w, 0, 0, E};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 9};
Line(9) = {9, 10};
Line(10) = {10, 1};

Line(11) = {2, 9};
Line(12) = {3, 8};
Line(13) = {4, 7};

Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

Plane Surface(1) = {1};
Line{11, 12, 13} In Surface {1};

Physical Surface("all") = {1};
Physical Line("top") = {5};
Physical Line("bottom") = {10};
Physical Line("right") = {6, 7, 8, 9};
Physical Line("left_lower") = {1};
Physical Line("left_middle") = {2, 3};
Physical Line("left_upper") = {4};
