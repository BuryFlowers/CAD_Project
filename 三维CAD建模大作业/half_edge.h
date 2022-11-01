#pragma once
#ifndef HALFEDGE
#define HALFEDGE

#include "vertex.h"
#include "edge.h"

class Loop;

class HalfEdge
{

public:

	HalfEdge* Eprevious;
	HalfEdge* Enext;
	HalfEdge* adj;
	Vertex* startV;
	Edge* edge;
	Loop* Wloop;
	/*HalfEdge(HalfEdge* Ep, HalfEdge* En, Vertex* s, Edge* e, Loop* Wl) {

		Eprevious = Ep;
		Enext = En;
		startV = s;
		edge = e;
		Wloop = Wl;

	}*/

	//void push();

private:

};


#endif // !HALFEDGE
