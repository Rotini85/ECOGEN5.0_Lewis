s=20.;

//Whole domain
Point(1) = {-3, -3, 0, s};
Point(4) = {20, -3, 0, s};
Point(5) = {20, 8, 0, s};
Point(6) = {-3, 8, 0, s};
Line(1) = {1, 4};
Line(2) = {4, 5};
Line(3) = {5, 6};
Line(4) = {6, 1};
Line Loop(1) = {3, 4, 1, 2};

//Physical lines and surface
Physical Line(1) = {4}; //Inlet
Physical Line(2) = {3, 1, 2}; //Outlet

Plane Surface(1) = {1};
Physical Surface(1) = {1};
