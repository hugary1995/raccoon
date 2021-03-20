// Gmsh project created on Wed Nov 11 14:32:31 2020
ta = 1.0; 
l = 0.5;
Point(1) = {-l,-l,0.0,ta};
Point(2) = {0, -l, 0.0, ta};
Point(3) = {l, -l, 0.0, ta};
Point(4) = {l, 0, 0.0, ta};
Point(5) = {l, l, 0.0, ta};
Point(6) = {0, l, 0.0, ta};
Point(7) = {-l, l, 0.0, ta};
Point(8) = {-l, 0.003, 0.0, ta};
Point(9) = {0, 0, 0, ta};
Point(10) = {-l, -0.003, 0.0, ta};


Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {10, 1};
Line(9) = {2, 9};
Line(10) = {4, 9};
Line(11) = {6, 9};
Line(12) = {8, 9};
Line(13) = {9, 10};

Transfinite Line {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} = 61 Using Progression 1;

Line Loop(5) = {1, 9, 13, 8};
Ruled Surface(6) = {5};
Transfinite Surface {6} = {1, 2, 9, 10};
Recombine Surface {6};

Line Loop(7) = {2, 3, 10, -9};
Ruled Surface(8) = {7};
Transfinite Surface {8} = {2, 3, 4, 9};
Recombine Surface {8};

Line Loop(9) = {-10, 4, 5, 11};
Ruled Surface(10) = {9};
Transfinite Surface {10} = {9, 4, 5, 6};
Recombine Surface {10};

Line Loop(11) = {12, -11, 6, 7};
Ruled Surface(12) = {11};
Transfinite Surface {12} = {8, 9, 6, 7};
Recombine Surface {12};

Physical Surface("top") = {60, 104};
Physical Surface("bottom") = {30, 74};
Physical Surface("left") = {42, 64};
Physical Surface("right") = {78, 100};
//Physical Line(22) = {6, 7, 8, 10, 11, 1, 2, 3, 4, 5};
//Physical Surface(23) = {17, 15, 19, 21};