#pragma once
#ifndef FACE
#define FACE

#include "loop.h"
#include "surface.h"

class Solid;

class Face
{

public:

	Face* preF;
	Face* nextF;
	Loop* Floop;
	Solid* Fsolid;
	//Surface* surface;

	void push(Loop* lp);

private:

};

#endif // !FACE



