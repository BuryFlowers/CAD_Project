#pragma once
#ifndef VERTEX
#define VERTEX

#include <iostream>

struct point {

	float x, y, z;
	point() {

		x = y = z = 0.0;

	}
	point(float t1, float t2, float t3) {

		x = t1;
		y = t2;
		z = t3;

	}

};

class HalfEdge;

class Vertex
{

public:

	/*Vertex(point v1, point v2, point v3, Vertex* pre, Vertex* nex, HalfEdge* he) {

		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
		preV = pre;
		nextV = nex;
		VHE = he;

	}*/

	point v;
	Vertex* nextV;
	Vertex* preV;
	HalfEdge* VHE;
	inline float x() { return v.x; }
	inline float y() { return v.y; }
	inline float z() { return v.z; }

	//void push(Vertex* v);

private:

};


#endif // !VERTEX
