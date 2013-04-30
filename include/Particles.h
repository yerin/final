//Class that defines the properties of the cloth particles

#ifndef Particles_H
#define Particles_H

#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

class Particles{

public:
	Vector3f position;
	Vector3f velocity;
	Vector3f normal;

	float mass;

};

#endif