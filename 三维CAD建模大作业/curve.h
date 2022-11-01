#pragma once
#ifndef CURVE
#define CURVE

#include "vertex.h"
#include <cmath>

struct Curve
{

	point s, t;
	Curve(point S, point T) {

		s = S;
		t = T;

	}
	float length() {

		return sqrt((s.x - t.x) * (s.x - t.x) + (s.y - t.y) * (s.y - t.y) + (s.z - t.z) * (s.z - t.z));

	}

};

#endif // !CURVE


