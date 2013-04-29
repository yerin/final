#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include <vector>
#include <Eigen/Dense>
#include "Particles.h"
using namespace Eigen;

struct spring{
	float damper;
	float springConstant;
	float restLength;
	float currentLength;
};

class cConstraints{
public:
	
	cConstraints();
	~cConstraints();

	Vector3f gravity;

	float newVelocity();
	float newAcceleration();
	float totalForce(Particles, Particles);
	void updateParticle();



};


#endif
