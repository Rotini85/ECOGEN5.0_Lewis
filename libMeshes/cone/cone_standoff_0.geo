// ---------- PARAMETERS ----------

// 1) Geometry parameters

// Manual values definition
coneRadius = 0.01536;
bubbleRadius = 0.0001;
thicknessTip = bubbleRadius;
thicknessOutlet = thicknessTip / (bubbleRadius / coneRadius);

// 2) Mesh size control
// Base (unstructured)
dxTip = 0.000002; // Size left points       [-]
dxOutlet = 0.001; // Size left points       [-]

// 3) Boundary conditions
bcSymmetry = 1;
bcConeWall = 2;
bcConeOutlet = 3;

// ---------- GEO ----------

// Define exterior domain
Point(1) = {0, 0, 0, dxTip};
Point(2) = {coneRadius, 0, 0, dxOutlet};
Point(3) = {coneRadius, thicknessOutlet, 0, dxOutlet};

pointSetoff = 4;
numPoints = 100;
teta = Atan(1.);
For i In {0:numPoints}
  Point(pointSetoff + i) = {bubbleRadius * Cos(teta * i / numPoints),
    bubbleRadius * Sin(teta * i / numPoints),
    0.,
    dxTip
  };
EndFor

Line(1) = {1, pointSetoff};
Line(2) = {pointSetoff, 2};
Line(3) = {2, 3};
Line(4) = {3, pointSetoff + numPoints};
Line(5) = {pointSetoff + numPoints, 1};
Spline (6) = {pointSetoff:pointSetoff+numPoints};

Line Loop(1) = {1, 6, 5};
Line Loop(2) = {2, 3, 4, -6};
Plane Surface(1) = {1};
Plane Surface(2) = {2};

// ---------- BOUNDARY CONDITIONS ----------

Physical Line(bcSymmetry) = {1, 2};
Physical Line(bcConeWall) = {4, 5};
Physical Line(bcConeOutlet) = {3};
surfaceFluide = 10;
Physical Surface(surfaceFluide) = {1, 2};
