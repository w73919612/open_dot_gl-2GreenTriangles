#pragma once
#include "ShapeData.hpp"

class ShapeGenerator
{
public:
	static ShapeData makeTriangle();
	static ShapeData makeTriangles();
	static ShapeData makeCube();
	static ShapeData makePyramid();
	static ShapeData makeArrow();
	static ShapeData makePlane(int dimensions);
	static ShapeData makePlaneVerts(int dimensions);
	static ShapeData makePlaneIndices(int dimensions); 
};

