#pragma once
#ifndef SURFACE
#define SURFACE

#include "vertex.h"
#include <cmath>

struct Vec3 {

	float x, y, z;
	Vec3() {

		x = y = z = 0.0;

	}
	Vec3(float v1, float v2, float v3) {

		x = v1;
		y = v2;
		z = v3;

	}
	inline float length() { return sqrt(x * x + y * y + z * z);  }

	void normalize() {

		float l = this->length();
		if (l == 0.0) return;
		x /= l;
		y /= l;
		z /= l;

	}

	inline Vec3 Cross(Vec3 n) {

		return Vec3(this->y * n.z - this->z * n.y, this->z * n.x - this->x * n.z, this->x * n.y - this->y * n.x);

	}

};

struct Surface {

	point p;
	Vec3 normal;
	Surface() {

		p = point();
		normal = Vec3(0.0, 0.0, 1.0);

	}
	Surface(point P, Vec3 N) {

		p = P;
		normal = N;

	}

};

#endif // !SURFACE

