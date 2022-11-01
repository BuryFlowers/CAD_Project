#include "solid.h"
#include "face.h"
#include "loop.h"
#include "half_edge.h"
#include "vertex.h"

Vertex* Vtail = NULL;
HalfEdge* HEtail = NULL;
Loop* LPtail = NULL;
Face* Ftail = NULL;
Solid* Stail = NULL;

extern void printLoop(Loop* lp);

Solid* mvsf(point p, Vertex* &v) {

	Face* f = new Face();
	Solid* s = new Solid();
	v = new Vertex();
	Loop* lp = new Loop();

	s->Sfaces = f;f->Fsolid = s;
	f->Floop = lp; lp->Lface = f;
	lp->Ledge = NULL;

	v->v = p;

	return s;
	

}

HalfEdge* mev(Vertex* v1, Loop* lp, point p) {

	Solid* s = lp->Lface->Fsolid;
	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	Edge* eg = new Edge();
	Vertex* v2 = new Vertex();

	he1->startV = v1;
	he2->startV = v2;
	he1->Enext = he2;
	he2->Eprevious = he1;
	he1->Wloop = he2->Wloop = lp;
	he1->adj = he2;
	he2->adj = he1;
	eg->HE1 = he1;
	eg->HE2 = he2;
	he1->edge = he2->edge = eg;

	if (lp->Ledge == NULL) {

		he2->Enext = he1;
		he1->Eprevious = he2;
		lp->Ledge = he1;

	}
	else {

		HalfEdge* he = lp->Ledge;
		//printLoop(lp);
		for (; he->Enext->startV != v1; he = he->Enext);

		he1->Eprevious = he;
		he2->Enext = he->Enext;
		he->Enext->Eprevious = he2;
		he->Enext = he1;

	}

	v2->v = p;
	eg->curve =new Curve(v1->v, v2->v);

	s->InsertEdge(eg);

	return he2;

}
Face* mef(Vertex* v1, Vertex* v2, Loop* lp) {

	Solid* s = lp->Lface->Fsolid;
	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	Edge* eg = new Edge();
	Loop* innerL = new Loop();
	Face* f = new Face();

	he1->startV = v1;
	he2->startV = v2;
	he1->adj = he2;
	he2->adj = he1;
	eg->HE1 = he1;
	eg->HE2 = he2;
	he1->edge = he2->edge = eg;

	HalfEdge* he = lp->Ledge;
	HalfEdge* heTail, * heStart;
	for (; he->startV != v1; he = he->Enext);
	heTail = he;
	for (; he->startV != v2; he = he->Enext);
	heStart = he;
	for (; he->startV != v2; he = he->Enext);
	if (heStart != he) innerL->isInner = true, heStart = he;
	
	he1->Enext = heStart;
	he1->Eprevious = heTail->Eprevious;
	he2->Enext = heTail;
	he2->Eprevious = heStart->Eprevious;

	heStart->Eprevious->Enext = he2;
	heStart->Eprevious = he1;
	heTail->Eprevious->Enext = he1;
	heTail->Eprevious = he2;

	innerL->Ledge = he2;
	he2->Wloop = innerL;
	lp->Ledge = he1;
	he1->Wloop = lp;

	f->push(innerL);
	innerL->Lface = f;
	s->push(f);
	f->Fsolid = s;

	eg->curve = new Curve(v1->v, v2->v);
	s->InsertEdge(eg);

	return f;

}

Loop* kemr(Vertex* v1, Vertex* v2, Loop* innerLp) {

	HalfEdge* he1 = innerLp->Ledge;
	//for (; (he1->startV != v2) || (he1->Enext->startV != v1); he1 = he1->Enext);
	while ((he1->startV != v2) || (he1->Enext->startV != v1)) {

		he1 = he1->Enext;

	}
	HalfEdge* he2 = he1->adj;
	Loop* lp = new Loop();
	Edge* eg = he1->edge;
	Face* f = innerLp->Lface;
	Solid* s = f->Fsolid;
	
	he1->Enext->Eprevious = he2->Eprevious;
	he2->Eprevious->Enext = he1->Enext;

	he1->Eprevious->Enext = he2->Enext;
	he2->Enext->Eprevious = he1->Eprevious;

	lp->Ledge = he1->Enext->Eprevious;
	innerLp->Ledge = he1->Eprevious;
	lp->Lface = f;
	f->push(lp);

	for (int i = 0; i < s->Sedges.size(); i++)
		if (s->Sedges[i] == eg) {
	
			s->Sedges.erase(s->Sedges.begin() + i);
			break;

		}

	delete eg;
	delete he1;
	delete he2;

	return lp;
}

void kfmrh(Loop* l1, Loop* l2) {

	Face* f1 = l1->Lface;
	Face* f2 = l2->Lface;

	f1->push(l2);
	l2->Lface = f1;

	Solid* s = f2->Fsolid;
	if (f2->nextF == NULL) f2->preF->nextF = NULL;
	else if (f2->preF == NULL) s->Sfaces = f2->nextF, f2->nextF->preF = NULL;
	else {

		f2->preF->nextF = f2->nextF;
		f2->nextF->preF = f2->preF;

	}

	delete f2;

}

void Sweep(Loop* lp, Vec3 d) {

	HalfEdge* he = lp->Ledge;
	Vertex* firstV = he->startV;
	Vertex* firstUp = mev(firstV, lp, point(firstV->v.x + d.x, firstV->v.y + d.y, firstV->v.z + d.z))->startV;
	Vertex* preUp = firstUp;
	he = he->Enext;
	Vertex* nextV = he->startV;
	while (nextV != firstV) {

		Vertex* Up = mev(nextV, lp, point(nextV->v.x + d.x, nextV->v.y + d.y, nextV->v.z + d.z))->startV;
		mef(preUp, Up, lp);
		preUp = Up;
		he = he->Enext;
		nextV = he->startV;
		//printLoop(lp);

	}
	Face* f = mef(preUp, firstUp, lp);

}