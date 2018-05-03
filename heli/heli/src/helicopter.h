/*
 * helicopter.h
 *
 *  Created on: Apr 12, 2018
 *      Author: 44857713
 */

#ifndef HELICOPTER_H_
#define HELICOPTER_H_

// This creates a new helicopter class that may be used as an object. It allows use within other
// classes if the header is called. The variables and functions are declared here, but they are
// initialised within the cpp file.
class helicopter {
public:
	helicopter();
	virtual ~helicopter();
	float fuelY, spin, centre_x, centre_y, rotate, mouse_x, mouse_y, rotate2,
			speed, length, scaleV, screenWidth, screenHeight;
	int delta_t;
	bool moveAllowed, fuelAllowed, startFuelling;
	void shadow(float cx, float cy, float r, int num_segments);
	void drawFuel(float X);
	void drawHeli();
	void smallFire(GLfloat x, GLfloat y, GLfloat r);
	void drawSmallFire();
	void startFuel();
	void startMove();
	// Position and orientation

};

#endif /* HELICOPTER_H_ */
