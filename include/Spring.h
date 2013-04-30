#ifndef SPRING_H
#define SPRING_H

#include <vector>
#include <Eigen/Dense>
#include "Particles.h"
using namespace Eigen;


class Spring{
public:

	int p1, p2;
	float tension;
	float springConstant;
	float restLength;

	Spring(Particles p1, Particles p2){
		Vector3f v = p1.position-p2.position;
		restLength=((float)sqrt( v(0) * v(0) + v(1) * v(1) + v(2) * v(2) ));
	}

};


#endif
