#pragma once
#ifndef EDGE
#define EDGE

#include "curve.h"

class HalfEdge;

class Edge
{

public:

	/*Edge(Curve* c, HalfEdge* he1, HalfEdge* he2, Solid* s) {

		curve = c;
		HE1 = he1;
		HE2 = he2;
		if (s != NULL) s->InsertEdge(this);

	}*/
	Curve* curve;
	HalfEdge* HE1;
	HalfEdge* HE2;

	inline float length() { return curve->length(); }

private:

};


#endif // !EDGE


