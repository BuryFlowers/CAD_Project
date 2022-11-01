#pragma once
#ifndef LOOP
#define LOOP

#include "half_edge.h"

class Face;

class Loop
{

public:
	Loop* preL;
	Loop* nextL;
	HalfEdge* Ledge;
	Face* Lface;
	bool isInner = false;
	/*Loop(Loop* p, Loop* n, HalfEdge* L, Face* Lf) {

		preL = p;
		nextL = n;
		Ledge = L;
		Lface = Lf;

	}*/

	//void push(Loop* lp);

private:

};


#endif // !LOOP


