#pragma once
#ifndef SOLID
#define SOLID

#include "face.h"
#include <vector>

using namespace std;

class Solid
{

public:

	Solid* preS;
	Solid* nextS;
	Face* Sfaces;
	vector<Edge*> Sedges;
	/*Solid(Solid* p, Solid* n, Face* Sf) {

		preS = p;
		nextS = n;
		Sfaces = Sf;
		Sedges.clear();

	}*/
	void InsertEdge(Edge* e) { Sedges.push_back(e); }
	void push(Face* f);

private:

};


#endif // !SOLID


