Wall=1;
Inlet=2;
surfFluid=10;

Point(1) = {0, 0, 0};
Point(2) = {0.05, 0, 0};
Point(3) = {0.055, 0.0, 0};
Point(4) = {0.105, 0.0, 0};
Point(5) = {0.105, 0.105, 0};
Point(6) = {0.0, 0.105, 0};
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,5};
Line(5) = {5,6};
Line(6) = {6,1};

Line Loop(7) = {1,2,3,4,5,6};
Plane Surface(8) = {7};

//Physical groups
Physical Surface(surfFluid)={8};
Physical Line(Inlet)={2};
Physical Line(Wall)={1,3,4,5,6};

//Mesh parameters
MeshSize{1,2,3,4,5,6}=0.001;
