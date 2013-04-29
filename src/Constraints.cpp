#include "Constraints.h"
#include "Particle.h"
#include <vector>
#include <cmath>

using namespace Eigen;
using namespace std;

cConstraints::cConstraints(){
}

cConstraints::~cConstraints() {
}

float newVelocity(){
	return 0.0;
}

float newAcceleration(float force){
	return 0.0;
}

float totalForce(Particles p1, Particles p2){
	spring s;

	//spring force
	float sForce = (-1*s.springConstant)*(s.restLength-s.currentLength);

	//damping force
	float dForce = (-1*s.damper)*(p1.velocity-p2.velocity);

	//spring-damper force
	float netForce = sForce+dForce;
	return netForce
}

void updateParticle(){

}

