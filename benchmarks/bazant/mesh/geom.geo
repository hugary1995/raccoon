sizing = 10e0;

Point(1) = {-1000, 0, 0, sizing};
Point(2) = {1000, 0, 0, sizing};
Point(3) = {1000, 50, 0, sizing};
Point(4) = {-1000, 50, 0, sizing};


// Lines
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};
Line Loop(1) = {1, 2, 3, 4};
Plane Surface(1) = {1};

// Names
Physical Surface("domain") = {1};
Physical Line("center") = {1};
Physical Line("right") = {2};
Physical Line("top") = {3};
Physical Line("left") = {4};
