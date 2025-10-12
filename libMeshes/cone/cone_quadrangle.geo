// ---------- PARAMETERS ----------

// 1) Geometry parameters

// Manual values definition
coneRadius = 0.01536;
tipRadius = 0.0001;
thicknessTip = tipRadius; // thicknessTip = tipRadius / 5;
thicknessOutlet = thicknessTip / (tipRadius / coneRadius);

// 2) Mesh size control
// Base (unstructured), not used here
dxTip = 0.00000125; // Size left points       [-]
dxOutlet = 0.001;   // Size left points       [-]

// Structured mesh
nbxBubble = 61;
nbxWater = 127;
nby = 55;

// 3) Boundary conditions
bcSymmetry = 1;
bcConeWall = 2;
bcConeOutlet = 3;

// ---------- GEO ----------

// Define exterior domain
Point(1) = {tipRadius, 0, 0, dxTip};
Point(2) = {tipRadius, thicknessTip, 0, dxTip};
Point(3) = {coneRadius, thicknessOutlet, 0, dxOutlet};
Point(4) = {coneRadius, 0, 0, dxOutlet};
Point(5) = {0, 0, 0, dxTip};

For i In {1 : 3}
  Line(i) = {i, i+1};
EndFor
Line(4) = {4, 1};
Line(5) = {5, 2};
Line(6) = {1, 5};

Line Loop(1) = {1, 2, 3, 4};
Line Loop(2) = {5, -1, 6};
Plane Surface(1) = {1};
Plane Surface(2) = {2};

// Structured mesh
Transfinite Line {5, 6} = nbxBubble;
Transfinite Line {2, -4} = nbxWater Using Progression 1.05;
Transfinite Line {1, 3} = nby;
Transfinite Surface "*";
Recombine Surface "*";

// ---------- BOUNDARY CONDITIONS ----------

Physical Line(bcSymmetry) = {4, 6};
Physical Line(bcConeWall) = {2, 5};
Physical Line(bcConeOutlet) = {3};
surfaceFluide = 10;
Physical Surface(surfaceFluide) = {1, 2};
